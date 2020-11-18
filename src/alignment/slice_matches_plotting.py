import sys
import os

sys.path.insert(0, os.path.abspath("../analysis/"))

from Plot import Plot
from utils import removePythonCache
import os

RELATIVE_PATH_TO_SLICE_MATCHES_DIR = '../../output/alignment/'


def ask_file_name():
    all_slice_matches_files = os.listdir(RELATIVE_PATH_TO_SLICE_MATCHES_DIR)
    print('Enter file number:')
    print('------------------')
    for i in range(len(all_slice_matches_files)):
        print('{}\t| {}'.format(i + 1, all_slice_matches_files[i]))
    print('------------------')
    try:
        file_index = int(input()) - 1
        if file_index < 0:
            raise IndexError
        return all_slice_matches_files[file_index]
    except ValueError:
        print('Wrong input format - value error')
        exit()
    except IndexError:
        print('File with this number does not exist - index error')
        exit()


def get_path(file_name):
    return RELATIVE_PATH_TO_SLICE_MATCHES_DIR + file_name


def make_plot(slice_matches_file_path):
    with open(slice_matches_file_path, 'r', encoding="utf-8") as input_file:
        _, query_name = map(str.strip, input_file.readline().split(':', 1))
        _, target_name = map(str.strip, input_file.readline().split(':', 1))

        query_name, target_name = query_name.strip(), target_name.strip()

        print("Query: {}".format(query_name))
        print("Reference: {}".format(target_name))

        plot = Plot("Slice matches plot", 8, grid_size=None, figsize=(10, 7), nameX=query_name, nameY=target_name)

        for line in input_file.read().split('\n'):
            if not line:
                continue

            block, positions = map(str.strip, line.split(':'))

            query_positions, target_positions = map(str.strip, positions.split(';'))

            query_positions = list(map(int, query_positions.split(',')))
            target_positions = list(map(int, target_positions.split(',')))

            block_size = len(block)

            for query_pos in query_positions:
                for target_pos in target_positions:
                    plot.line(abs(query_pos) + (block_size if target_pos < 0 else 0),
                              abs(target_pos) + (block_size if target_pos < 0 else 0),
                              abs(query_pos) + (block_size if target_pos >= 0 else 0),
                              abs(target_pos) + (block_size if target_pos >= 0 else 0),
                              color="#000")

    plot.show()
    del plot


if __name__ == "__main__":
    removePythonCache("../../")

    needed_file_name = ask_file_name()
    file_path = get_path(needed_file_name)
    make_plot(file_path)

    removePythonCache("../../")
