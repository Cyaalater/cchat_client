project('CChat','c')
copy = find_program('copy.py')

run_command(
    copy,
    join_paths('src', 'main.glade'),
    join_paths('resources','main.glade')
)
run_command(
    copy,
    join_paths('src', 'chat.glade'),
    join_paths('resources','chat.glade')
)



incdir = include_directories('include')
src = ['include/sockcom/sockcom.c','include/namecom/namecom.c']
gtk_dep = dependency('gtk+-3.0')
json_dep = dependency('cJSON')

executable('main','src/main.c',
    dependencies: [gtk_dep,json_dep],
    include_directories : incdir,
    sources : src)