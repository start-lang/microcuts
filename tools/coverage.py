#!/usr/bin/env python3
import sys
import os
import re


if __name__ == '__main__':
    """Show .gcov file statistics."""
    multiline_section = True if '-m' in sys.argv else False
    header = '{:15} {:>6} {:>6} {:>8} {:>10}'.format('Name', 'Lines', 'Miss',
                                                     'Cover', 'Missing')
    print()
    print(header)
    t_line = '-' * len(header)
    print(t_line)

    total_line_count = 0
    total_miss = 0
    total_cover = 0
    for file in os.listdir('.'):
        if file.endswith('.gcov'):
            lines = open(file).readlines()
            not_empty = 0
            miss = 0
            miss_sec = []
            for line in lines:
                search = re.search(r'#####:\s*?(\d+)', line)
                if search:
                    miss += 1
                    line_n = int(search.group(1))
                    if len(miss_sec) > 0 and miss_sec[-1][1] == line_n - 1:
                        miss_sec[-1][1] = line_n
                    else:
                        miss_sec.append([line_n, line_n])
                elif '    -:' not in line:
                    not_empty += 1
            line_count = miss + not_empty
            file_coverage = (float(not_empty) / (line_count)) * 100
            file = file[:-5]
            if len(file) > 15:
                file = file[:(15 - 3)] + '...'
            n_sections = ''
            for a, b in miss_sec:
                n_sections += f'{a}' if a == b else f'{a}-{b}'
                if multiline_section:
                    n_sections += f'\n{" ":{len(header) - 9}}'
                else:
                    n_sections += ', '
            if multiline_section:
                n_sections = n_sections[:n_sections.rfind('\n')]
            else:
                n_sections = n_sections[:-2]
            print(f'{file:15s} {line_count:6d} {miss:6d}',
                  f'{file_coverage:7.02f}%  {n_sections}')
            total_line_count += line_count
            total_miss += miss
            total_cover += file_coverage * line_count
    total_cover /= total_line_count
    print(t_line)
    print(f'{"TOTAL":15} {total_line_count:6d} {total_miss:6d}',
          f'{total_cover:7.2f}%')
    print()
