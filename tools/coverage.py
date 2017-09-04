#!/usr/bin/env python
import sys
import os
import re


if __name__ == '__main__':
    """Show .gcov file statistics."""
    multiline_section = True if '-m' in sys.argv else False
    cols = ('Name', 'Lines', 'Miss', 'Cover', 'Missing')
    ts = [15, 6, 6, 8, 10] # table spaces
    t_header = '%%-%ds %%%ds %%%ds %%%ds  %%-%ds' % tuple(ts)
    t_line = '-' * (sum(ts) + 4)
    ts[3] -= 1
    t_row = '%%-%ds %%%dd %%%dd %%%d.2f%%%%  %%-%ds' % tuple(ts)
    total_line_count = 0
    total_miss = 0
    total_cover = 0
    print t_header % cols
    print t_line
    for file in os.listdir('.'):
        if file.endswith('.gcov'):
            lines = open(file).readlines();
            not_empty = 0
            miss = 0
            miss_sec = []
            for line in lines:
                search = re.search('#####:\s*?(\d+)', line)
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
            if len(file) > ts[0]:
                file = file[:(ts[0] - 3)] + '...'
            n_sections = ''
            for sec in miss_sec:
                if multiline_section:
                    n_sections += ('%d\n' % sec[0]) if sec[0] == sec[1] else ('%d-%d\n' % tuple(sec))
                    n_sections += (sum(ts) - 4) * ' '
                else:
                    n_sections += ('%d, ' % sec[0]) if sec[0] == sec[1] else ('%d-%d, ' % tuple(sec))
            if multiline_section:
                n_sections = n_sections[:n_sections.rfind('\n')]
            print t_row % (file, line_count, miss, file_coverage, n_sections[:-2])
            total_line_count += line_count
            total_miss += miss
            total_cover += file_coverage * line_count
    print t_line
    total_cover /= total_line_count
    print t_row % ('TOTAL', total_line_count, total_miss, total_cover, '')
