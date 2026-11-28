#!/usr/bin/env python3
"""Проверка формата, нумерации, наполнения и границ полей. Рендер PDF через MuPDF."""
import argparse
from pathlib import Path
import subprocess
from zipfile import ZipFile

from docx import Document
from lxml import etree
from pypdf import PdfReader

ROOT = Path(__file__).resolve().parents[1]
REPORTS = ROOT / 'docs' / 'reports'
WORK = ROOT / 'build-docs' / 'qa'
A4 = (595.276, 841.890)
NAMES = {
    'Гармаев_КР': 'main',
    'АБс-525_Курсовая_ГармаевА.В.': 'checkpoint',
    'Гармаев_Пояснительная_записка': 'note',
    'Разбор_кода_LinuxSandbox': 'code',
    'Подготовка_к_защите_LinuxSandbox': None,
}
W = {'w': 'http://schemas.openxmlformats.org/wordprocessingml/2006/main'}
COURSEWORK = {'main', 'checkpoint', 'note'}
PLAN_TITLE = 'Таблица 1. Состав и содержание работ по созданию системы'


def check_word(path, key):
    toc = {}
    with ZipFile(path) as package:
        assert package.testzip() is None, path
        body = etree.fromstring(package.read('word/document.xml'))
        if key in COURSEWORK:
            contents = ' '.join(body.xpath('//w:t/text()', namespaces=W)).casefold()
            assert not any(word in contents for word in ('чекпоинт', 'codex', 'chatgpt')), path
            fonts = set(body.xpath('//w:rFonts/@w:ascii | //w:rFonts/@w:hAnsi', namespaces=W))
            assert fonts == {'Times New Roman'}, (path, fonts)
        if key == 'main':
            number_fields = sum(len(etree.fromstring(package.read(name)).xpath('//w:fldSimple[@w:instr="PAGE"]', namespaces=W))
                                for name in package.namelist() if name.startswith('word/header') and name.endswith('.xml'))
            assert number_fields == 1, path
            assert body.xpath('//w:sectPr/w:pgNumType/@w:start', namespaces=W) == ['2'], path
            assert len(body.xpath('//w:pPr/w:pBdr/w:bottom', namespaces=W)) >= 13, path
            italic = body.xpath('//w:r[w:rPr/w:i[not(@w:val="0")]]/w:t/text()', namespaces=W)
            for name in ('Гармаеву Амгалану Владимировичу', 'Архипова Анастасия Борисовна', 'Защита информации'):
                assert name in italic, (path, name)
        if key == 'note':
            assert body.xpath('//w:instrText[contains(., "TOC")]', namespaces=W), path
            for link in body.xpath('//w:hyperlink[starts-with(@w:anchor, "section_")]', namespaces=W):
                name = link.get('{%s}anchor' % W['w'])
                text = link.getparent().xpath('./w:r/w:t/text()', namespaces=W)
                toc[name] = int(text[-1])
    doc = Document(path)
    for section in doc.sections:
        sizes = (section.page_width.mm, section.page_height.mm, section.left_margin.mm, section.right_margin.mm,
                 section.top_margin.mm, section.bottom_margin.mm)
        assert all(abs(actual - expected) < 0.1 for actual, expected in zip(sizes, (210, 297, 30, 15, 20, 20))), path
    style = doc.styles['Normal']
    assert style.font.name == 'Times New Roman' and style.font.size.pt == 14, path
    assert abs(style.paragraph_format.first_line_indent.mm - 12.5) < 0.1, path
    assert abs(style.paragraph_format.line_spacing - 1.5) < 0.01, path
    if key == 'checkpoint':
        assert len(doc.inline_shapes) == 8, path
    if key == 'main':
        assert len(doc.sections) == 5, path
        assert any(p.text == PLAN_TITLE for p in doc.paragraphs), path
        plan = next(t for t in doc.tables if t.rows[0].cells[0].text == 'Этап')
        assert len(plan.rows) == 7 and all(not row.cells[3].text for row in plan.rows[1:]), path
        assert [row.cells[0].text for row in plan.rows[1:]] == [
            '1 Анализ задания', '2 Проектирование', '3 Разработка', '4 Испытания и отладка',
            '5 Оформление', '6 Сдача и защита',
        ], path
    return toc


def read_lines(page):
    for line in page.xpath('.//line'):
        chars = line.xpath('.//char')
        text = ''.join(c.get('c') for c in chars).strip()
        if text:
            quads = [tuple(map(float, c.get('quad').split())) for c in chars if not c.get('c').isspace()]
            _, y0, _, y1 = map(float, line.get('bbox').split())
            xs = [value for quad in quads for value in quad[::2]]
            yield text, (min(xs), y0, max(xs), y1)


def check_toc(path, reader, pages, toc):
    destinations = reader.named_destinations
    assert toc and set(toc) == set(destinations), path
    assert {key: reader.get_destination_page_number(value) + 1 for key, value in destinations.items()} == toc, path
    written = []
    linked = []
    for index in range(1, toc['section_1'] - 1):
        for annotation in reader.pages[index].get('/Annots', []):
            target = annotation.get_object().get('/Dest')
            if target in toc and target not in linked:
                linked.append(target)
        written.extend(int(text) for text, (x0, y0, _, y1) in read_lines(pages[index])
                       if x0 > 528 and 44 < y0 < y1 < 790 and text.isdecimal())
    assert len(linked) == len(toc) and written == [toc[target] for target in linked], path
    print(f'Оглавление: {len(toc)} записей сопоставлены со страницами и ссылками PDF.')


def check_pdf(path, key, render, toc):
    out = WORK / key
    out.mkdir(parents=True, exist_ok=True)
    xml = subprocess.run(['mutool', 'draw', '-q', '-F', 'stext', str(path)], check=True, capture_output=True).stdout
    (out / 'text.xml').write_bytes(xml)
    pages = etree.fromstring(xml).findall('page')
    reader = PdfReader(path)
    assert len(pages) == len(reader.pages), path
    complete = '\n'.join(text for page in pages for text, _ in read_lines(page))
    if key in COURSEWORK:
        font_types = set()
        for pdf_page in reader.pages:
            for resource in pdf_page['/Resources'].get('/Font', {}).get_object().values():
                font = resource.get_object()
                font_types.add(str(font['/BaseFont']))
                assert '/FontFile2' in font['/DescendantFonts'][0].get_object()['/FontDescriptor'], (path, font)
        assert font_types and all('timesnewroman' in ''.join(x for x in name.casefold() if x.isalpha())
                                  for name in font_types), (path, font_types)
        assert not any(word in complete.casefold() for word in ('чекпоинт', 'codex', 'chatgpt')), path
    for i, (page, pdf_page) in enumerate(zip(pages, reader.pages), 1):
        assert all(abs(float(actual) - expected) < 0.15
                   for actual, expected in zip((pdf_page.mediabox.width, pdf_page.mediabox.height), A4)), (path, i)
        lines = list(read_lines(page))
        assert any(y0 > 44 and y1 < 800 and len(text) > 10 for text, (_, y0, _, y1) in lines), (path, i, 'пустая страница')
        for text, (x0, y0, x1, y1) in lines:
            assert 0 <= x0 < x1 <= A4[0] and 0 <= y0 < y1 <= A4[1], (path, i, text)
            # MuPDF иногда включает в рамку несколько пунктов выносных элементов глифа.
            assert 82 <= x0 and x1 <= 556, (path, i, text, 'горизонтальные поля')
        if key == 'main':
            numbers = [(text, (x0 + x1) / 2) for text, (x0, y0, x1, _) in lines if y0 < 48 and text.isdecimal()]
            if i <= 4:
                assert not numbers, (path, i)
            else:
                assert len(numbers) == 1 and numbers[0][0] == str(i - 3), (path, i, numbers)
                assert abs(numbers[0][1] - A4[0] / 2) < 3, (path, i, numbers)
            if i in {1, 4}:
                assert any(text == 'Новосибирск 2026' and 750 <= box[1] < box[3] <= 786
                           for text, box in lines), (path, i, 'город и год')
        if key == 'checkpoint':
            operations = pdf_page.get_contents().operations
            assert sum(operator == b'Do' for _, operator in operations) == 2, (path, i, 'число изображений')
    if key == 'checkpoint':
        assert 4 <= len(pages) <= 6, path
        assert all(f'Рисунок {i}' in complete for i in range(1, 9)), path
    if key == 'main':
        assert all(f'К{i:02d}' in complete for i in range(1, 13)), path
        assert all(f'Таблица А.{i}' in complete for i in range(1, 9)), path
        assert '15.12.2026' in complete, path
        assert '28.11.2026' not in complete and PLAN_TITLE in ' '.join(complete.split()), path
    if key == 'note':
        assert 'СОДЕРЖАНИЕ' in complete and 'Список использованных источников' in complete, path
        check_toc(path, reader, pages, toc)
    if render:
        subprocess.run(['mutool', 'draw', '-q', '-r', '100', '-o', str(out / 'page-%02d.png'), str(path)],
                       check=True, capture_output=True)
    print(f'{path.name} — {len(pages)} стр., А4; границы текста и контрольные элементы проверены')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--render', action='store_true')
    args = parser.parse_args()
    for name, key in NAMES.items():
        toc = check_word(REPORTS / (name + '.docx'), key)
        if key is not None:
            check_pdf(REPORTS / (name + '.pdf'), key, args.render, toc)
    print('Пять DOCX: формат страниц, поля, стиль и специальные элементы проверены по XML.')


if __name__ == '__main__':
    main()
