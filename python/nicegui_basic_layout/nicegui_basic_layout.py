#! /usr/bin/python3
from nicegui import ui


@ui.page('/')
async def page_layout():
    ui.label('CONTENT')
    [ui.label(f'Line {i}') for i in range(100)]
    with ui.header(elevated=True).style('background-color: #3874c8').classes('items-center justify-between'):
        ui.label('HEADER')
        ui.button(on_click=lambda: right_drawer.toggle()).props('flat color=white icon=menu')
    with ui.left_drawer(top_corner=True, bottom_corner=True).style('background-color: #d7e3f4'):
        ui.label('LEFT DRAWER')
        [ui.label(f'OPTION {i}') for i in range(100)]
    with ui.right_drawer(fixed=False).style('background-color: #ebf1fa').props('bordered') as right_drawer:
        ui.label('RIGHT DRAWER')
    with ui.footer().style('background-color: #3874c8'):
        ui.label('FOOTER')


ui.run()

