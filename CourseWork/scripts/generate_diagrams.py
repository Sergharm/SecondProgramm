"""Печать редактируемых схем Graphviz в SVG и PNG."""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
DIAGRAMS = ROOT / 'docs' / 'diagrams'


def render_diagrams():
    for source in sorted(DIAGRAMS.glob('*.dot')):
        command = ['neato', '-n2'] if source.stem == 'modules' else ['dot']
        for extension in ('svg', 'png'):
            subprocess.run([*command, f'-T{extension}', str(source), '-o', str(source.with_suffix('.' + extension))], check=True)


if __name__ == '__main__':
    render_diagrams()
