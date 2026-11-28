#!/usr/bin/env python3
"""Обновление документации курсовой работы LinuxSandbox в DOCX и PDF."""
import argparse
from datetime import date, datetime
from html import escape
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
from zipfile import ZipFile

from docx import Document
from docx.enum.section import WD_SECTION_START
from docx.enum.table import WD_CELL_VERTICAL_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.opc.constants import RELATIONSHIP_TYPE
from docx.shared import Mm, Pt, RGBColor
from lxml import etree, html
import markdown
from PIL import Image

sys.dont_write_bytecode = True

from generate_diagrams import render_diagrams
from report_content import assignment_first, assignment_second, coursework_cover, p, specification_tables, technical_cover

ROOT = Path(__file__).resolve().parents[1]
DOCS = ROOT / 'docs'
REPORTS = DOCS / 'reports'
SOURCES = DOCS / 'report_sources'
DIAGRAMS = DOCS / 'diagrams'
SHOTS = DOCS / 'screenshots'
WORK = ROOT / 'build-docs'
TEMPLATE = DOCS / 'templates' / 'Гармаев_КР_исправленный.docx'
FONT = 'Times New Roman'
FALLBACK = 'Liberation Serif'
LOCAL_TIMES = {
    ('normal', '400'): 'Times.TTF',
    ('normal', '700'): 'Timesbd.TTF',
    ('italic', '400'): 'Timesi.TTF',
    ('italic', '700'): 'Timesbi.TTF',
}
CITY = 'Новосибирск 2026'
WIDTH = 165
ALIGN = {'left': WD_ALIGN_PARAGRAPH.LEFT, 'center': WD_ALIGN_PARAGRAPH.CENTER,
         'right': WD_ALIGN_PARAGRAPH.RIGHT, 'justify': WD_ALIGN_PARAGRAPH.JUSTIFY}

# Размер, интервал, отступы до и после, абзацный отступ и выравнивание.
PARAGRAPHS = {
    'mast': (12, 1.4, 0, 7, 0, 'center'),
    'ministry': (11, 1.15, 5, 19, 0, 'center'),
    'university-first': (10.3, 1.15, 0, 16, 0, 'center'),
    'university': (11, 1.15, 0, 3, 0, 'center'),
    'university-name': (11, 1.15, 10, 3, 0, 'center'),
    'department': (13, 1.15, 21, 2, 0, 'center'),
    'logo': (12, 1, 37, 56, 0, 'center'),
    'cover-title': (16, 1.15, 6, 4, 0, 'center'),
    'cover-sub': (14, 1.15, 1, 4, 0, 'center'),
    'cover-topic': (14, 1.2, 1, 5, 0, 'center'),
    'approval': (12, 1.45, 27, 5, 0, 'right'),
    'assignment-title': (14, 1.12, 26, 14, 0, 'center'),
    'plan-space': (14, 1.15, 29, 8, 0, 'center'),
    'agreed': (14, 1.2, 26, 15, 0, 'center'),
    'form-line': (12, 1.16, 7, 1, 0, 'left'),
    'form-hint': (9, 1.05, 0, 4, 0, 'center'),
    'form-title': (13, 1.14, 14, 7, 0, 'center'),
    'technical-title': (16, 1.15, 79, 5, 0, 'center'),
    'discipline': (14, 1.15, 59, 2, 0, 'center'),
    'flat': (14, 1.4, 0, 3, 0, 'left'),
    'compact': (11, 1.15, 0, 3, 0, 'left'),
    'table-caption': (12, 1.15, 10, 3, 0, 'left'),
    'figure-caption': (12, 1.15, 4, 0, 0, 'center'),
    'image': (12, 1, 0, 0, 0, 'center'),
    'figure-ref': (11, 1.15, 0, 9, 0, 'center'),
    'placeholder': (13, 1.3, 0, 8, 0, 'center'),
    'toc-title': (16, 1.15, 0, 10, 0, 'center'),
    'toc': (12, 1.2, 0, 2, 0, 'left'),
    'code': (9, 1.15, 4, 5, 0, 'left'),
}

SCREENSHOTS = [
    ('01_menu.png', 'Главное меню LinuxSandbox v0.1'),
    ('02_select.png', 'Выбор комплектного файла test_tcp'),
    ('03_info.png', 'Свойства исполняемого файла test_tcp'),
    ('04_analysis.png', 'Завершение изолированного анализа test_tcp'),
    ('05_tcp.png', 'Результаты мониторинга TCP-сокетов'),
    ('06_udp.png', 'Результаты мониторинга UDP-сокетов'),
    ('07_report.png', 'Сформированный текстовый отчёт LinuxSandbox'),
]


def font_available():
    result = subprocess.run(['fc-match', '-f', '%{family}', FONT], check=True, capture_output=True, text=True)
    return FONT in result.stdout.split(',')


def prep_logo():
    logo = WORK / 'logo.jpeg'
    with ZipFile(TEMPLATE) as package:
        logo.write_bytes(package.read('word/media/image1.jpeg'))
    return logo


def md(path):
    return markdown.markdown(path.read_text(encoding='utf-8'), extensions=['tables', 'fenced_code', 'sane_lists'])


def figure(path, caption, number, height=110, commentary=None, role='figure'):
    if path is None:
        payload = p('Снимок ожидается после успешного испытания в изолированной среде.', 'placeholder')
    else:
        with Image.open(path) as img:
            w, h = img.size
        print_width = min(WIDTH, height * w / h)
        payload = f'<p class="image"><img src="{path}" data-mm="{print_width:.3f}" alt="{escape(caption)}"></p>'
    label = f'Рисунок {number} — {caption}'
    note = p(commentary, 'figure-ref') if commentary else ''
    return f'<div class="{role}">{payload}{p(label, "figure-caption")}{note}</div>'


def checkpoint():
    pictures = [
        figure(SHOTS / filename if (SHOTS / filename).is_file() else None, caption, number, 99,
               None if (SHOTS / filename).is_file() else 'См. условия повторной съёмки в SCREENSHOT_GUIDE.md.',
               role='checkpoint-figure')
        for number, (filename, caption) in enumerate(SCREENSHOTS, 1)
    ]
    pictures.append(figure(DIAGRAMS / 'modules.png', 'Схема взаимодействия программных модулей LinuxSandbox', 8, 98,
                           role='checkpoint-figure'))
    return [('plain', pictures[i * 2] + pictures[i * 2 + 1]) for i in range(4)]


def source_sections(logo):
    spec = md(SOURCES / 'TECHNICAL_SPEC.md')
    for name, value in specification_tables().items():
        spec = spec.replace(f'<div data-table="{name}"></div>', value)
    note = md(SOURCES / 'EXPLANATORY_NOTE.md')
    figures = {'architecture': ('Общая архитектура LinuxSandbox v0.1', 1),
               'algorithm': ('Алгоритм сетевого анализа', 2)}
    for key, (caption, number) in figures.items():
        note = note.replace(f'<div data-figure="{key}"></div>', figure(DIAGRAMS / f'{key}.png', caption, number))
    code = md(DOCS / 'CODE_GUIDE.md').replace('<div data-figure="sequence"></div>',
        figure(DIAGRAMS / 'sequence.png', 'Последовательность вызовов при анализе', 1, 78))
    return [
        ('Гармаев_КР', [('cover', coursework_cover(logo)), ('front', assignment_first()), ('front', assignment_second()),
                       ('tech', technical_cover()), ('numbered', spec)]),
        ('АБс-525_Курсовая_ГармаевА.В.', checkpoint()),
        ('Гармаев_Пояснительная_записка', [('cover', coursework_cover(logo, note=True)),
                                       ('bottom', '<div data-toc="yes"></div><div class="page-break"></div>' + note)]),
        ('Разбор_кода_LinuxSandbox', [('bottom', code)]),
        ('Подготовка_к_защите_LinuxSandbox', [('bottom', md(DOCS / 'DEFENSE_GUIDE.md'))]),
    ]


def prepare_sections(sections):
    result = []
    headings = []
    for mode, content in sections:
        elements = html.fragments_fromstring(content)
        for element in elements:
            for header in element.iter('h1', 'h2'):
                anchor = f'section_{len(headings) + 1}'
                header.set('id', anchor)
                headings.append((anchor, header.text_content(), int(header.tag[1])))
        result.append((mode, elements))
    return result, headings


CSS = '''
@page { size: A4 portrait; margin: 20mm 15mm 20mm 30mm }
@page cover { @bottom-center { content: 'Новосибирск 2026'; font-size: 12pt; vertical-align: top; margin-top: 0.5mm; transform: translateY(-16pt) } }
@page tech { counter-reset: page 1; @bottom-center { content: 'Новосибирск 2026'; font-size: 12pt; vertical-align: top; margin-top: 0.5mm; transform: translateY(-16pt) } }
@page numbered { @top-center { content: counter(page); font-size: 12pt; transform: translateX(-7.5mm) } }
@page bottom { @bottom-center { content: counter(page); font-size: 12pt } }
@page front { }
@page plain { }
body { margin: 0; color: black; font-size: 14pt; line-height: 1.5; text-align: justify }
.page-block + .page-block { break-before: page }
p { margin: 0 0 3pt; text-indent: 12.5mm; orphans: 2; widows: 2 }
.center { text-align: center; text-indent: 0 }
.strong { font-weight: bold }
h1, h2, h3, h4 { font-size: 14pt; font-weight: bold; line-height: 1.25; margin: 10pt 0 4pt; break-after: avoid; text-indent: 12.5mm; text-align: left }
.role-space { height: 110pt }
.page-break { break-before: page }
.keep-block, .figure, .checkpoint-figure { break-inside: avoid }
.checkpoint-figure + .checkpoint-figure { margin-top: 10mm }
.checkpoint-figure { margin-top: 3mm }
.placeholder { border: 0.6pt solid #777; padding: 26mm 7mm; margin-top: 3mm }
.figure { margin: 7pt 0 }
.image { break-after: avoid }
.table-caption { break-after: avoid }
p img { vertical-align: middle; max-width: 100%; height: auto }
table { width: 100%; table-layout: fixed; border-collapse: collapse; font-size: 11pt; line-height: 1.18; text-align: left; margin: 0 0 6pt }
thead { display: table-header-group }
tr { break-inside: avoid }
td, th { vertical-align: top; border: 0.5pt solid black; padding: 3pt 4pt; overflow-wrap: anywhere }
th { text-align: center }
.plain td, .roles td, .form-grid td { border: none; font-size: 12pt; padding: 0 4pt }
.form-grid { margin-bottom: 0 }
.form-line { border-bottom: 0.5pt solid black; min-height: 16pt; break-after: avoid }
.form-line span { padding-right: 1mm }
.spec-plan td, .spec-plan th, .calendar td, .calendar th,
.calendar-first td, .calendar-first th { vertical-align: middle }
.calendar-first tbody tr:first-child td { text-align: center }
.keep-text { break-inside: avoid }
.plain td:last-child { text-align: right }
.toc-entry { display: block; font-size: 12pt; margin: 1pt 0; line-height: 1.2 }
.toc-entry.level-2 { margin-left: 6mm }
.toc-entry::after { content: leader('.') target-counter(attr(href), page) }
a { color: black; text-decoration: none; overflow-wrap: anywhere }
pre { margin: 4pt 0 6pt; white-space: pre-wrap; overflow-wrap: anywhere; font-size: 9pt; line-height: 1.14; text-align: left; padding: 3pt 6pt; border-left: 0.6pt solid #777 }
code { font: inherit }
pre code { font-family: 'DejaVu Sans Mono' }
ul, ol { text-align: left; margin: 4pt 0 5pt; padding-left: 18mm }
li { margin-bottom: 2pt }
'''


def html_document(title, sections, headings, font, local_fonts):
    style = CSS + f"body {{font-family: '{font}'}} @page {{font-family: '{font}'}}"
    for (slant, weight), path in local_fonts.items():
        style += (f"\n@font-face {{ font-family: '{FONT}'; src: url('{path.as_uri()}'); "
                  f"font-style: {slant}; font-weight: {weight} }}")
    for name, (size, line, before, after, indent, alignment) in PARAGRAPHS.items():
        style += (f'\n.{name} {{font-size:{size}pt; line-height:{line}; margin-top:{before}pt; '
                  f'margin-bottom:{after}pt; text-indent:{indent}mm; text-align:{alignment}}}')
    style += ''.join(f'\n.page-{mode} {{page: {mode}}}' for mode in ('cover', 'front', 'numbered', 'tech', 'bottom', 'plain'))
    body = []
    for mode, elements in sections:
        # Копия HTML получает ширины объектов и ссылки для печатного оглавления.
        content = '\n'.join(html.tostring(e, encoding='unicode') for e in elements)
        sec = html.fragment_fromstring(content, create_parent='section')
        sec.set('class', f'page-block page-{mode}')
        for img in sec.iter('img'):
            img.set('style', f'width:{img.get("data-mm")}mm')
            img.set('src', Path(img.get('src')).as_uri())
        for table in sec.iter('table'):
            widths = table.get('data-widths')
            if widths:
                cols = etree.Element('colgroup')
                for width in widths.split(','):
                    etree.SubElement(cols, 'col', style=f'width:{width}%')
                table.insert(0, cols)
        toc = sec.find('.//div[@data-toc]')
        if toc is not None:
            toc.append(html.fragment_fromstring(p('СОДЕРЖАНИЕ', 'toc-title strong')))
            for anchor, label, level in headings:
                etree.SubElement(toc, 'a', {'class': f'toc-entry level-{level}', 'href': f'#{anchor}'}).text = label
        body.append(html.tostring(sec, encoding='unicode'))
    return f'<!doctype html><html lang="ru"><head><meta charset="utf-8"><title>{escape(title)}</title><style>{style}</style></head><body>{"".join(body)}</body></html>'


def render_html(markup):
    import pydyf
    from weasyprint import HTML
    from weasyprint.text.fonts import FontConfiguration

    # Совместимость WeasyPrint 62 с переименованиями операторов pydyf 0.12.
    for old, new in {'transform': 'set_matrix', 'text_matrix': 'set_text_matrix',
                     'color_space': 'set_color_space', 'shading': 'paint_shading'}.items():
        if not hasattr(pydyf.Stream, old):
            setattr(pydyf.Stream, old, getattr(pydyf.Stream, new))
    return HTML(string=markup).render(font_config=FontConfiguration())


def element(tag, **attributes):
    node = OxmlElement(f'w:{tag}')
    for key, value in attributes.items():
        node.set(qn(f'w:{key}'), str(value))
    return node


def set_font(font, size=None, family=FONT):
    font.name = family
    if size is not None:
        font.size = Pt(size)
    font.color.rgb = RGBColor(0, 0, 0)


def new_document(use_template):
    doc = Document(TEMPLATE) if use_template else Document()
    if use_template:
        body = doc._element.body
        for node in list(body):
            if node.tag != qn('w:sectPr'):
                body.remove(node)
        for node in list(body.sectPr):
            if node.tag in {qn('w:headerReference'), qn('w:footerReference'), qn('w:pgNumType'), qn('w:docGrid')}:
                body.sectPr.remove(node)
        for owner in (doc.part, doc.part.package):
            for rel_id, rel in list(owner.rels.items()):
                if rel.reltype.endswith(('/customXml', '/thumbnail', '/header', '/footer', '/image')):
                    owner.rels.pop(rel_id)
    for name in ('Normal', 'Title', 'Subtitle', 'Heading 1', 'Heading 2', 'Heading 3', 'Heading 4', 'Caption'):
        style = doc.styles[name]
        set_font(style.font, 14)
    normal = doc.styles['Normal'].paragraph_format
    normal.alignment = ALIGN['justify']
    normal.line_spacing = 1.5
    normal.first_line_indent = Mm(12.5)
    normal.space_before = Pt(0)
    normal.space_after = Pt(3)
    normal.keep_with_next = False
    normal.keep_together = False
    normal.widow_control = True
    for level in range(1, 5):
        style = doc.styles[f'Heading {level}']
        style.font.bold = True
        fmt = style.paragraph_format
        fmt.first_line_indent = Mm(12.5)
        fmt.alignment = ALIGN['left']
        fmt.space_before = Pt(10)
        fmt.space_after = Pt(4)
        fmt.line_spacing = 1.25
        fmt.page_break_before = False
        fmt.keep_with_next = True
    settings = doc.settings.element
    update = settings.find(qn('w:updateFields'))
    if update is None:
        settings.append(element('updateFields', val='true'))
    else:
        update.set(qn('w:val'), 'true')
    props = doc.core_properties
    for name in ('keywords', 'comments', 'category', 'subject', 'last_modified_by', 'identifier', 'version'):
        setattr(props, name, '')
    props.author = 'Гармаев Амгалан Владимирович'
    props.created = props.modified = datetime.combine(date(2026, 11, 28), datetime.min.time())
    return doc


def configure_section(section, mode, initial_page):
    section.page_width, section.page_height = Mm(210), Mm(297)
    section.left_margin, section.right_margin = Mm(30), Mm(15)
    section.top_margin = section.bottom_margin = Mm(20)
    section.header_distance = Mm(7)
    section.footer_distance = Mm(20 if mode in {'cover', 'tech'} else 7)
    section.different_first_page_header_footer = False
    for part in (section.header, section.footer):
        part.is_linked_to_previous = False
        paragraph = part.paragraphs[0]
        paragraph.clear()
        apply_paragraph(paragraph, (12, 1, 0, 0, 0, 'center'))
    if mode == 'numbered':
        section._sectPr.insert_element_before(element('pgNumType', start=2), 'w:cols', 'w:docGrid')
    if mode in {'numbered', 'bottom'}:
        part = section.footer if mode == 'bottom' else section.header
        number = part.paragraphs[0]
        if mode == 'numbered':
            number.paragraph_format.right_indent = Mm(15)
        field = element('fldSimple', instr='PAGE')
        cached_run = element('r')
        properties = element('rPr')
        properties.append(element('rFonts', ascii=FONT, hAnsi=FONT, cs=FONT))
        properties.append(element('sz', val=24))
        cached_run.append(properties)
        text = element('t')
        text.text = str(2 if mode == 'numbered' else initial_page)
        cached_run.append(text)
        field.append(cached_run)
        number._p.append(field)
    if mode in {'cover', 'tech'}:
        run = section.footer.paragraphs[0].add_run(CITY)
        set_font(run.font, 12)


def apply_paragraph(paragraph, style):
    size, line, before, after, indent, alignment = style
    pf = paragraph.paragraph_format
    pf.line_spacing = line
    pf.space_before, pf.space_after = Pt(before), Pt(after)
    pf.first_line_indent = Mm(indent)
    paragraph.alignment = ALIGN[alignment]
    for run in paragraph.runs:
        if run.font.size is None:
            run.font.size = Pt(size)
    return size


def add_link(paragraph, href, label, size):
    link = element('hyperlink')
    if href.startswith('#'):
        link.set(qn('w:anchor'), href[1:])
    elif href.startswith(('https://', 'http://')):
        rel = paragraph.part.relate_to(href, RELATIONSHIP_TYPE.HYPERLINK, is_external=True)
        link.set(qn('r:id'), rel)
    else:
        paragraph.add_run(label)
        return
    run = element('r')
    props = element('rPr')
    props.append(element('rFonts', ascii=FONT, hAnsi=FONT, cs=FONT))
    props.append(element('sz', val=size * 2))
    run.append(props)
    text = element('t')
    text.text = label
    run.append(text)
    link.append(run)
    paragraph._p.append(link)


def inline(paragraph, node, size, bold=False, italic=False):
    def add(text):
        if text:
            run = paragraph.add_run(text)
            set_font(run.font, size)
            run.bold, run.italic = bold, italic

    add(node.text)
    for child in node:
        if child.tag == 'br':
            paragraph.add_run().add_break()
        elif child.tag == 'img':
            paragraph.add_run().add_picture(child.get('src'), width=Mm(float(child.get('data-mm'))))
        elif child.tag == 'a':
            add_link(paragraph, child.get('href', ''), child.text_content(), size)
        else:
            inline(paragraph, child, size, bold or child.tag in ('strong', 'b'), italic or child.tag in ('em', 'i'))
        add(child.tail)


def word_table(doc, node):
    rows = node.xpath('./thead/tr | ./tbody/tr | ./tr')
    cols = len(rows[0])
    widths = [float(w) for w in node.get('data-widths', ','.join([str(100 / cols)] * cols)).split(',')]
    alignment = node.get('class')
    plain = alignment in {'plain', 'roles', 'form-grid'}
    table = doc.add_table(rows=0, cols=cols)
    table.autofit = False
    table.style = 'Normal Table' if plain else 'Table Grid'
    for col, width in zip(table.columns, widths):
        col.width = Mm(WIDTH * width / 100)
    for row_index, source in enumerate(rows):
        row = table.add_row()
        row._tr.get_or_add_trPr().append(element('cantSplit'))
        if source[0].tag == 'th':
            row._tr.get_or_add_trPr().append(element('tblHeader', val=1))
        for index, (cell, child, width) in enumerate(zip(row.cells, source, widths)):
            cell.width = Mm(WIDTH * width / 100)
            pp = cell.paragraphs[0]
            if alignment == 'form-grid':
                for paragraph_index, field_node in enumerate(child):
                    fill_word_paragraph(pp if paragraph_index == 0 else cell.add_paragraph(), field_node)
                continue
            if alignment in {'spec-plan', 'calendar', 'calendar-first'}:
                cell.vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.CENTER
            pos = 'right' if alignment == 'plain' and index == cols - 1 else 'center' if child.tag == 'th' else 'left'
            if alignment == 'calendar-first' and row_index == 1:
                pos = 'center'
            size = 12 if plain else 11
            apply_paragraph(pp, (size, 1.18, 2, 2, 0, pos))
            if alignment == 'spec-plan' and row_index < len(rows) - 1:
                pp.paragraph_format.keep_with_next = True
            inline(pp, child, size, child.tag == 'th')


def fill_word_paragraph(paragraph, node):
    styles = node.get('class', '').split()
    spec = next((PARAGRAPHS[key] for key in styles if key in PARAGRAPHS), None)
    size = apply_paragraph(paragraph, spec) if spec else 14
    if 'center' in styles:
        paragraph.alignment = ALIGN['center']
        paragraph.paragraph_format.first_line_indent = Mm(0)
    if any(key in styles for key in ('table-caption', 'image', 'placeholder', 'form-line')):
        paragraph.paragraph_format.keep_with_next = True
    if 'keep-text' in styles:
        paragraph.paragraph_format.keep_together = True
    if 'form-line' in styles:
        border = element('pBdr')
        border.append(element('bottom', val='single', sz=4, space=1, color='000000'))
        paragraph._p.get_or_add_pPr().insert_element_before(border, 'w:spacing', 'w:ind', 'w:jc')
    if 'placeholder' in styles:
        paragraph.paragraph_format.space_before = Pt(72)
        paragraph.paragraph_format.space_after = Pt(72)
    inline(paragraph, node, size, 'strong' in styles)


def word_toc(doc, headings, pages):
    paragraph = doc.add_paragraph()
    set_font(paragraph.add_run('СОДЕРЖАНИЕ').font, 16)
    paragraph.runs[0].bold = True
    apply_paragraph(paragraph, PARAGRAPHS['toc-title'])
    for i, (anchor, label, level) in enumerate(headings):
        pp = doc.add_paragraph()
        apply_paragraph(pp, PARAGRAPHS['toc'])
        pp.paragraph_format.left_indent = Mm(6 if level == 2 else 0)
        if i == 0:
            pp.add_run()._r.append(element('fldChar', fldCharType='begin', dirty='true'))
            instr = element('instrText')
            instr.text = ' TOC \\o "1-2" \\h \\z \\u '
            pp.add_run()._r.append(instr)
            pp.add_run()._r.append(element('fldChar', fldCharType='separate'))
        from docx.enum.text import WD_TAB_ALIGNMENT, WD_TAB_LEADER
        pp.paragraph_format.tab_stops.add_tab_stop(Mm(WIDTH - 1), WD_TAB_ALIGNMENT.RIGHT, WD_TAB_LEADER.DOTS)
        add_link(pp, '#' + anchor, label, 12)
        if anchor in pages:
            set_font(pp.add_run(f'\t{pages[anchor]}').font, 12)
        if i == len(headings) - 1:
            pp.add_run()._r.append(element('fldChar', fldCharType='end'))


def word_nodes(doc, elements, headings, pages):
    for node in elements:
        style = node.get('class', '').split()
        if node.tag == 'div':
            if node.get('data-toc'):
                word_toc(doc, headings, pages)
            elif 'page-break' in style:
                doc.add_page_break()
            elif 'role-space' in style:
                pp = doc.add_paragraph()
                apply_paragraph(pp, (1, 1, 0, 108, 0, 'left'))
            else:
                lead = None
                if 'checkpoint-figure' in style:
                    lead = doc.add_paragraph()
                    apply_paragraph(lead, (1, 1, 0, 11, 0, 'center'))
                    lead.paragraph_format.keep_with_next = True
                word_nodes(doc, list(node), headings, pages)
            continue
        if node.tag == 'table':
            word_table(doc, node)
            continue
        if node.tag in ('ul', 'ol'):
            for i, item in enumerate(node, 1):
                pp = doc.add_paragraph()
                apply_paragraph(pp, (14, 1.35, 0, 3, 0, 'left'))
                pp.paragraph_format.left_indent = Mm(9)
                pp.add_run('• ' if node.tag == 'ul' else f'{i}. ')
                inline(pp, item, 14)
            continue
        if node.tag in ('h1', 'h2', 'h3', 'h4'):
            pp = doc.add_paragraph(style=f'Heading {node.tag[1]}')
            anchor = node.get('id')
            if anchor:
                pp._p.append(element('bookmarkStart', id=anchor.split('_')[-1], name=anchor))
            inline(pp, node, 14, bold=True)
            if anchor:
                pp._p.append(element('bookmarkEnd', id=anchor.split('_')[-1]))
            continue
        pp = doc.add_paragraph()
        if node.tag == 'pre':
            apply_paragraph(pp, PARAGRAPHS['code'])
            run = pp.add_run(node.text_content().rstrip('\n'))
            set_font(run.font, 9, 'DejaVu Sans Mono')
            continue
        fill_word_paragraph(pp, node)


def write_docx(title, sections, headings, pages, target, use_template):
    doc = new_document(use_template)
    doc.core_properties.title = title
    last_mode = None
    for initial_page, (mode, elements) in enumerate(sections, 1):
        if last_mode is None:
            configure_section(doc.sections[0], mode, initial_page)
        elif mode != last_mode or mode != 'plain':
            section = doc.add_section(WD_SECTION_START.NEW_PAGE)
            configure_section(section, mode, initial_page)
        else:
            doc.add_page_break()
        word_nodes(doc, elements, headings, pages)
        last_mode = mode
    doc.save(target)


def export_lo(executable, input_doc, target_dir, profile):
    subprocess.run([executable, f'-env:UserInstallation={profile.as_uri()}', '--headless', '--convert-to',
                    'pdf:writer_pdf_Export', '--outdir', str(target_dir), str(input_doc)], check=True, timeout=90)
    pdf = target_dir / input_doc.with_suffix('.pdf').name
    if not pdf.is_file():
        raise RuntimeError(f'LibreOffice не создал {pdf.name}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--engine', choices=('auto', 'libreoffice', 'weasyprint'), default='auto')
    parser.add_argument('--only', choices=('main', 'checkpoint', 'note', 'code', 'defense'), nargs='+')
    parser.add_argument('--skip-diagrams', action='store_true')
    args = parser.parse_args()
    chosen = {'main', 'checkpoint', 'note', 'code', 'defense'} if args.only is None else set(args.only)
    WORK.mkdir(exist_ok=True)
    REPORTS.mkdir(exist_ok=True)
    SHOTS.mkdir(exist_ok=True)
    tempfile.tempdir = str(WORK)
    if not args.skip_diagrams:
        render_diagrams()
    logo = prep_logo()
    missing = [name for name, _ in SCREENSHOTS if not (SHOTS / name).is_file()]
    print('Отсутствующие снимки: ' + (', '.join(missing) if missing else 'нет'))
    libreoffice = shutil.which('libreoffice') or shutil.which('soffice')
    if args.engine == 'libreoffice' and not libreoffice:
        parser.error('LibreOffice не найден')
    system_font = font_available()
    local_fonts = {style: WORK / 'font-assets' / name for style, name in LOCAL_TIMES.items()}
    if not all(font.is_file() for font in local_fonts.values()):
        local_fonts = {}
    has_font = system_font or bool(local_fonts)
    if args.engine == 'libreoffice' and not system_font:
        parser.error(f'Установите {FONT} для экспорта через LibreOffice')
    if not has_font and chosen.intersection({'main', 'checkpoint', 'note'}):
        parser.error(f'Для отчётов требуется {FONT}. Используйте системный шрифт или комплект в build-docs/font-assets')
    font = FONT if has_font else FALLBACK
    print(f'DOCX: {FONT}. PDF: {font}.' if not has_font else f'DOCX и PDF: {FONT}.')
    use_lo = libreoffice and args.engine != 'weasyprint' and system_font
    with tempfile.TemporaryDirectory(prefix='export-', dir=WORK) as work:
        staging = Path(work)
        backups = None
        for key, (title, raw) in zip(('main', 'checkpoint', 'note', 'code', 'defense'), source_sections(logo)):
            if key not in chosen:
                continue
            sections, headings = prepare_sections(raw)
            pages = {}
            pdf = staging / (title + '.pdf')
            docx = staging / (title + '.docx')
            if (not use_lo and key != 'defense') or key == 'note':
                out = render_html(html_document(title, sections, headings, font, local_fonts))
                pages = {anchor: i for i, page in enumerate(out.pages, 1) for anchor in page.anchors}
                if not use_lo and key != 'defense':
                    out.write_pdf(pdf)
            write_docx(title, sections, headings, pages, docx, key == 'main')
            if key != 'defense' and use_lo:
                export_lo(libreoffice, docx, staging, staging / 'lo-profile')
            for output in (docx, pdf) if key != 'defense' else (docx,):
                final = REPORTS / output.name
                if final.is_file():
                    if backups is None:
                        stamp = datetime.now().strftime('%Y%m%d-%H%M%S-%f')
                        backups = WORK / 'backups' / stamp
                        backups.mkdir(parents=True)
                    shutil.copy2(final, backups / final.name)
                output.replace(final)
                print(final.relative_to(ROOT))


if __name__ == '__main__':
    main()
