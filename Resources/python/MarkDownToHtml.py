import markdown
import argparse
import os
import mdx_math

def convert_markdown_to_html(markdown_path, output_path):
    try:
        with open(markdown_path, 'r', encoding='utf-8') as md_file:
            markdown_text = md_file.read()
        md = markdown.Markdown(extensions=[
            "mdx_math",
            "markdown.extensions.codehilite",
            "markdown.extensions.meta",
            "markdown.extensions.toc",
            "markdown.extensions.abbr",
            "markdown.extensions.extra",
            "markdown.extensions.admonition",
            "markdown.extensions.attr_list",
            "markdown.extensions.def_list",
            "markdown.extensions.fenced_code",
            "markdown.extensions.footnotes",
            "markdown.extensions.legacy_attrs",
            "markdown.extensions.legacy_em",
            "markdown.extensions.md_in_html",
            "markdown.extensions.nl2br",
            "markdown.extensions.sane_lists",
            "markdown.extensions.smarty",
            "markdown.extensions.tables",
            "markdown.extensions.wikilinks"
        ], extension_configs={
            "mdx_math": {
                'enable_dollar_delimiter': True,
                'add_preview': True
            }
        })
        html_output = md.convert(markdown_text)
        with open(output_path, 'w', encoding='utf-8') as html_file:
            html_file.write(html_output)
        print(f"HTML file created at {output_path}")
    except Exception as e:
        print(f"An error occurred: {e}")


parser = argparse.ArgumentParser(description="Convert Markdown to HTML")
parser.add_argument("input_path", type=str, help="Path to the Markdown file")
parser.add_argument("output_path", type=str, help="Output HTML file path", default=None)
args = parser.parse_args()

if not args.output_path:
    base_name = os.path.splitext(args.input_path)[0]
    args.output_path = f"{base_name}.html"

convert_markdown_to_html(args.input_path, args.output_path)
