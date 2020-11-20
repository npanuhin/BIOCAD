from typing import List
from string import ascii_uppercase
from Bio.SeqIO.FastaIO import SimpleFastaParser
from json import load as json_load  # , dump as json_dump
from copy import deepcopy
import os

from utils import *

from Line import Line, shiftLines
from Plot import Plot
from Events import Pass, Rotation, Insertion, Deletion, Translocation  # , Duplication

ROOT_PATH = "../../"


# INT_MAX = int(1e9) + 7

# TODO:
# - Left bottom


# !!! X - query, Y - ref !!!

# Small
# SETTINGS = {
#     "grid_size": 100,
#     "min_rid_size": 1,
#     "dot_skip_rate": 1,
#     "dotsize": 0.1,
#     "fontsize": 10,
#     "figsize": (10, 7),

#     "min_event_size": 3,
#     "lines_join_size": 5,
#     "line_min_size": 10
# }

# Large
SETTINGS = {
    "grid_size": int(1e5),
    "min_rid_size": int(1e3),
    "dot_skip_rate": 10,
    "dotsize": 0.1,
    "fontsize": 8,
    "figsize": (10, 7),

    "min_event_size": int(5e3),
    "lines_join_size": "$min_event_size + 3",
    "line_min_size": "$min_event_size"
}


# /-----TESTING SETTINGS-----\ #

query_genome_path = "samples/large08/large_genome1.fasta"
ref_genome_path = "samples/large08/large_genome2.fasta"
sam_file_path = "BWA/large08/bwa_output.sam"
show_plot = True
output_folder = "output/analysis/large08"

# query_genome_path = "samples/small/source.fasta"
# ref_genome_path = "samples/small/duplication.fasta"
# sam_file_path = "BWA/small/duplication/bwa_output.sam"
# show_plot = True
# output_folder = "tests/small/duplication"

# \-----TESTING SETTINGS-----/ #


def analyze(query_genome_path: str, ref_genome_path: str, sam_file_path: str, show_plot: bool, output_folder: str, settings: dict):
    print("---| {} |---".format(output_folder))

    output_folder = mkpath(ROOT_PATH, output_folder)

    setSettings(settings, mkpath(output_folder, "settings.json"))

    with open(mkpath(ROOT_PATH, "src", "analysis", "STORAGE", "CIGAR_FLAGS.json"), 'r', encoding="utf-8") as file:
        CIGAR_FLAGS = json_load(file, encoding="utf-8")

    with open(mkpath(ROOT_PATH, query_genome_path), 'r', encoding="utf-8") as file:
        for name, sequence in SimpleFastaParser(file):
            query_genome_name = name
            query_genome_length = len(sequence)
            break

    with open(mkpath(ROOT_PATH, ref_genome_path), 'r', encoding="utf-8") as file:
        for name, sequence in SimpleFastaParser(file):
            ref_genome_name = name
            ref_genome_length = len(sequence)
            break

    print("Query: {} [{}]".format(query_genome_name, prtNum(query_genome_length)))
    print("Reference: {} [{}]\n".format(ref_genome_name, prtNum(ref_genome_length)))

    # return
# ====================================================================================================================================================================
    # Parse CIGAR and create a list of all actions
    print("Reading SAM file...")

    segments = []
    with open(mkpath(ROOT_PATH, sam_file_path), 'r', encoding="utf-8") as sam_file:
        for line in (line.strip().split() for line in sam_file if not line.strip().startswith("@")):
            # Quality:
            # mapQ = int(line[4])
            # quality = round((10 ** (mapQ / -10)) * 100, 6)

            # Flags:
            flags_bit = int(line[1])
            flags = set()
            for i in range(len(CIGAR_FLAGS) - 1, -1, -1):
                cur_flag, flags_bit = divmod(flags_bit, 2 ** i)
                if cur_flag:
                    flags.add(i)

            # Rid:
            rid_size = len(line[9])
            if rid_size <= settings["min_rid_size"]:
                continue

            # CIGAR:
            actions = []
            buff = ""
            for char in line[5]:
                if char in ascii_uppercase:
                    actions.append([int(buff), char])
                    buff = ""
                else:
                    buff += char

            rotated = lambda ref_pos: ref_genome_length - ref_pos if 4 in flags else ref_pos

            # Start position:
            cur_query_pos = int(line[3])
            cur_ref_pos = 0

            for length, action_type in actions:

                if action_type in ('S', 'H'):
                    cur_ref_pos += length

                elif action_type == 'M':
                    segments.append([cur_query_pos, cur_ref_pos, length, (4 in flags)])
                    cur_query_pos += length
                    cur_ref_pos += length

                elif action_type == 'I':
                    cur_ref_pos += length

                elif action_type == 'D':
                    cur_query_pos += length

    # return
# ====================================================================================================================================================================
    # Creating plot
    print("Creating plot...")

    plot = Plot("Main plot", settings["fontsize"], settings["grid_size"], settings["figsize"], query_genome_name, ref_genome_name)
    plot.legendLine({
        "Insertion": "#0f0",
        "Deletion": "#f00",
        "Duplication": "#f0f",
        "Translocation": "#0ff"
    }, fontsize=settings["fontsize"], lw=2)

    # return
# ====================================================================================================================================================================
    # Creating dots
    print("Creating dots...", end="")

    graph = [[] for _ in range(query_genome_length + 1)]
    count = 0

    for cur_query_pos, cur_ref_pos, length, rotated in segments:
        if rotated:
            cur_ref_pos = ref_genome_length - cur_ref_pos
        for _ in range(length):
            graph[cur_query_pos].append(cur_ref_pos)
            cur_query_pos += 1
            cur_ref_pos += (-1 if rotated else 1)

        count += length

    del segments

    print(" {}".format(prtNum(count)))  # Can be with optional compress: count // N

    # return
# ====================================================================================================================================================================
    # Counting lines
    print("Counting lines...", end="")

    lines_join_size2 = settings["lines_join_size"] ** 2
    line_min_size2 = settings["line_min_size"] ** 2

    lines = []

    for x in range(0, len(graph), settings["dot_skip_rate"]):
        for y in graph[x]:
            for line in lines:
                if distance2(x, y, *line.dots[-1]) <= lines_join_size2 and \
                        (len(line.dots) == 1 or distance2(x, y, *line.dots[-2]) <= lines_join_size2):
                    line.dots.append([x, y])
                    break
            else:
                lines.append(Line(dots=[[x, y]]))

    for line in lines:
        line.dots.sort()

        line.start_x, line.start_y = line.dots[0]
        line.end_x, line.end_y = line.dots[-1]

        if len(line.dots) >= 2:
            k, b = linearApproxDots(line.dots)         # \
            line.start_y = int(k * line.start_x + b)   # |--> Approximation  TODO: int
            line.end_y = int(k * line.end_x + b)       # /

        # line[4] = line[4][::settings["dot_skip_rate"]]  # Optional compress

    lines = [line for line in lines if distance2(line.start_x, line.start_y, line.end_x, line.end_y) >= line_min_size2]

    lines.sort(key=lambda line: (line.start_x, line.start_y))

    print(" {} lines".format(len(lines)))
    print("Lines:", *lines, sep='\n')

    # return
# ====================================================================================================================================================================
    # Shift and rotations
    print("\nCounting shift and rotations...")

    not_shited_lines = deepcopy(lines)

    def countMetric(lines):
        result = 0

        # First option:
        # k, b = linearApproxLines(lines)
        # main_line = Line(0, b, query_genome_length, query_genome_length * k + b)

        # Second option:
        # main_line = Line(0, 0, query_genome_length, query_genome_length) -> Second "for" option

        # Third option: TODO - approx with k: 1, b: search

        # Fourth option: TODO - approx with k: search, b: 0

        # First "for" option:
        # for line in lines:
        #     result += int((line.start_y - YCoordOnLine(*main_line.coords, line.start_x)) ** 2)
        #     result += int((line.end_y - YCoordOnLine(*main_line.coords, line.end_x)) ** 2)

        # Second "for" option:
        for line in lines:
            result += int((line.start_y - line.start_x) ** 2) + int((line.end_y - line.end_x) ** 2)

        return result

    def countMetricWithRotation(lines, rotation, apply_rotation=False) -> int:
        rotation_center = (
            min(
                lines[rotation.start_line].start_y, lines[rotation.start_line].end_y,
                lines[rotation.end_line].start_y, lines[rotation.end_line].end_y
            ) + max(
                lines[rotation.start_line].start_y, lines[rotation.start_line].end_y,
                lines[rotation.end_line].start_y, lines[rotation.end_line].end_y
            )
        ) // 2

        for line_index in range(rotation.start_line, rotation.end_line + 1):
            lines[line_index].rotateY(rotation_center)

        metric_value = countMetric(lines)

        if not apply_rotation:
            for line_index in range(rotation.start_line, rotation.end_line + 1):
                lines[line_index].rotateY(rotation_center)

        if apply_rotation:
            rotation.rotation_center = rotation_center
            for line_index in range(rotation.start_line, rotation.end_line + 1):
                lines[line_index].rotateY(rotation_center, line=False, dots=True)

        return metric_value

    def countBestRotations(rotated_lines) -> List[Line]:
        possible_rotations = [
            Rotation(start_line, end_line)
            for start_line in range(len(rotated_lines))
            for end_line in range(start_line, len(rotated_lines))
        ]
        # print("\nPossible rotations:", *possible_rotations, sep='\n')

        cur_metric_value = countMetric(rotated_lines)
        rotation_actions = []

        # if draw:
        #     fastDrawLines(plot, rotated_lines, save=mkpath(output_folder, "history", "x0.png"))
        #     index = 1

        while True:
            best_metric_value = float('inf')
            best_rotation_index = 0

            for i, rotation in enumerate(possible_rotations):
                # TODO: WORKAROUND #1
                min_line_center, max_line_center = float('inf'), float("-inf")
                for line_index in range(rotation.start_line, rotation.end_line + 1):
                    min_line_center = min(min_line_center, rotated_lines[line_index].center_y)
                    max_line_center = max(max_line_center, rotated_lines[line_index].center_y)
                bad = False
                for line_index in range(len(rotated_lines)):
                    if not (rotation.start_line <= line_index <= rotation.end_line) and \
                            min_line_center < rotated_lines[line_index].center_y < max_line_center:
                        bad = True
                if bad:
                    continue

                # TODO: WORKAROUND-CONDITION #2
                if rotated_lines[rotation.start_line].isTiltedCorrectly() or rotated_lines[rotation.end_line].isTiltedCorrectly():
                    continue

                cur_metric = countMetricWithRotation(rotated_lines, rotation)

                if cur_metric < best_metric_value:
                    best_metric_value = cur_metric
                    best_rotation_index = i

            if best_metric_value >= cur_metric_value:
                break

            cur_metric_value = countMetricWithRotation(rotated_lines, possible_rotations[best_rotation_index], apply_rotation=True)

            # print("\n{} -> {}".format(possible_rotations[best_rotation_index], cur_metric_value))
            # print("best_metric_value = {}".format(best_metric_value))
            # print("best_rotation_index = {}".format(best_rotation_index))

            rotation_actions.append(possible_rotations[best_rotation_index])

            # if draw:
            #     fastDrawLines(plot, rotated_lines, save=mkpath(output_folder, "history", "x{}.png".format(index)))

        # print("Final metric value: ", cur_metric_value, countMetric(rotated_lines))
        print("\nRotation actions:", *rotation_actions, sep='\n')
        # print("\nRotated lines:", *rotated_lines, sep='\n')

        # fastDrawLines(plot, rotated_lines, show=True)

        return cur_metric_value, rotated_lines, rotation_actions

    def countShift(lines, start_line, apply_shift=False) -> int:
        d_x = lines[start_line].start_x

        for line_index in range(start_line, len(lines)):
            lines[line_index].shift(dx=-d_x)

        for line_index in range(0, start_line):
            lines[line_index].shift(dx=query_genome_length - d_x)

        # print("\nLines:", *lines, sep='\n')

        rotated_lines = shiftLines(deepcopy(lines), start_line)
        # print("\nRotated lines:", *rotated_lines, sep='\n')

        metric_value, rotated_lines, rotation_actions = countBestRotations(rotated_lines)

        print("metric_value = {} or {}".format(metric_value, metric_value * len(rotation_actions)))

        if apply_shift:
            return shiftLines(lines, start_line), rotated_lines, rotation_actions

        for line_index in range(start_line, len(lines)):
            lines[line_index].shift(dx=d_x)

        for line_index in range(0, start_line):
            lines[line_index].shift(dx=d_x - query_genome_length)

        return metric_value * len(rotation_actions)

    best_metric_value = float("inf")
    best_metric_value_start_line = 0

    for start_line in range(len(lines)):
        print("\n-| Counting with start_line = {}...".format(start_line))
        cur_metric_value = countShift(lines, start_line)

        if cur_metric_value < best_metric_value:
            best_metric_value = cur_metric_value
            best_metric_value_start_line = start_line

    print("\n===| Counting end result with start_line = {}...".format(best_metric_value_start_line))
    lines, rotated_lines, rotation_actions = countShift(lines, best_metric_value_start_line, apply_shift=True)

    # plot.clear()
    # for line in lines:
    #     plot.plotLine(line)
    # plot.show()
    # plot.clear()

    print("\nLines:", *lines, sep='\n')
    print("\nRotated lines:", *rotated_lines, sep='\n')

    # return
# ====================================================================================================================================================================
    # Insertions and deletions
    print("\nCounting insertions and deletions...")

    spaceless_lines = deepcopy(rotated_lines)

    insertion_actions = []
    cur_pos, last_line_end = 0, 0
    for line in sorted(spaceless_lines, key=lambda line: line.start_y):
        if line.start_y > cur_pos:
            insertion_actions.append(Insertion(cur_pos, line.start_y - cur_pos))
        elif line.start_y < cur_pos:
            pass  # TODO: duplication Y

        cur_pos = max(cur_pos, line.end_y)
        line.shift(dy=last_line_end - line.start_y)
        last_line_end = line.end_y

    deletion_actions = []
    cur_pos, last_line_end = 0, 0
    for line in sorted(spaceless_lines, key=lambda line: line.start_x):
        if line.start_x > cur_pos:
            deletion_actions.append(Deletion(cur_pos, line.start_x - cur_pos))
        elif line.start_x < cur_pos:
            pass  # TODO: duplication X

        cur_pos = max(cur_pos, line.end_x)
        line.shift(dx=last_line_end - line.start_x)
        last_line_end = line.end_x

    large_insertion_actions = [action for action in insertion_actions if action.size >= settings["min_event_size"]]
    large_deletion_actions = [action for action in deletion_actions if action.size >= settings["min_event_size"]]

    print("\nLarge insertions:", *large_insertion_actions, sep='\n')
    print("\nLarge deletions:", *large_deletion_actions, sep='\n')

    # return
# ====================================================================================================================================================================
    # Translocations
    print("\nCounting translocations....")

    translocation_actions = []

    def recountPos(end_line):
        last_x = spaceless_lines[end_line - 1].end_x if end_line > 0 else None
        last_y = spaceless_lines[end_line - 1].end_y if end_line > 0 else None
        next_x = spaceless_lines[end_line + 1].start_x if end_line < len(spaceless_lines) - 1 else None
        next_y = spaceless_lines[end_line + 1].start_y if end_line < len(spaceless_lines) - 1 else None
        return last_x, last_y, next_x, next_y

    start_line = 0

    while start_line < len(spaceless_lines):
        end_line = start_line
        last_x, last_y, next_x, next_y = recountPos(end_line)

        while end_line < len(spaceless_lines) and (
            (last_x is not None and not equalE(last_x, spaceless_lines[end_line].start_x, 3)) or
            (last_y is not None and not equalE(last_y, spaceless_lines[end_line].start_y, 3)) or
            (next_x is not None and not equalE(next_x, spaceless_lines[end_line].end_x, 3)) or
            (next_y is not None and not equalE(next_y, spaceless_lines[end_line].end_y, 3))
        ):
            end_line += 1
            last_x, last_y, next_x, next_y = recountPos(end_line)

        if end_line - start_line > 1:
            order = [(i, lines[i]) for i in range(start_line + 1, end_line - 1)]
            order.sort(key=lambda item: item[1].center_y)
            translocation_actions.append(Translocation(start_line + 1, end_line - 2, [i for i, line in order]))

        start_line = end_line + 1

    print("\nTranslocations:", *translocation_actions, sep='\n')

    # return
# ====================================================================================================================================================================
    # Plotting dots, lines and large actions
    print("\nPlotting dots, lines and large actions...")

    for insertion in large_insertion_actions:
        plot.poligon([
            (0, insertion.start_y), (0, insertion.start_y + insertion.height),
            (query_genome_length, insertion.start_y + insertion.height), (query_genome_length, insertion.start_y)
        ], color=(0, 1, 0, 0.3))

    for deletion in large_deletion_actions:
        plot.poligon([
            (deletion.start_x, 0), (deletion.start_x, ref_genome_length),
            (deletion.start_x + deletion.length, ref_genome_length), (deletion.start_x + deletion.length, 0)
        ], color=(1, 0, 0, 0.3))

    # for line in lines:
    #     plot.plotLine(line, color="#fa0")
    #     plot.scatter(line.dots[::settings["dot_skip_rate"]], dotsize=settings["dotsize"], color="#00f")

    for line in rotated_lines:
        plot.plotLine(line)

    for line in spaceless_lines:
        plot.plotLine(line, color="#0ff")

    # for line in not_shited_lines:
    #     plot.scatter(line.dots[::settings["dot_skip_rate"]], dotsize=settings["dotsize"], color="#eee")

    print("Saving plot...")
    # plot.tight()
    plot.save(mkpath(output_folder, "sam_analyze.png"))

    if show_plot:
        print("Showing plot...")
        plot.show()

    plot.clear()

    # return
# ====================================================================================================================================================================
    # Make and save history
    print("Making history...", end="")

    if not os.path.exists(mkpath(output_folder, "history")):
        os.mkdir(mkpath(output_folder, "history"))

    for filename in os.listdir(mkpath(output_folder, "history")):
        os.remove(mkpath(output_folder, "history", filename))

    large_actions = [Pass()] + rotation_actions + \
        sorted(large_insertion_actions + large_deletion_actions, key=lambda action: -action.size) + translocation_actions

    print(" {} records\n".format(len(large_actions)))
    # print("Large actions:", *large_actions, sep='\n')

    history_text_output = []
    for action in large_actions:

        if isinstance(action, Rotation):
            history_text_output.append("Rotation from {} (Query) to {} (Query)".format(
                prtNum(int(lines[action.start_line].start_x)),
                prtNum(int(lines[action.end_line].end_x))
            ))

        elif isinstance(action, Deletion):
            history_text_output.append("Deletion of {}-{} (Query)".format(
                prtNum(int(action.start_x)),
                prtNum(int(action.start_x + action.length))
            ))

        elif isinstance(action, Insertion):
            history_text_output.append("Insertion of {}-{} (Ref)".format(
                prtNum(int(action.start_y)),
                prtNum(int(action.start_y + action.height))
            ))

        elif isinstance(action, Translocation):
            history_text_output.append("Translocation (COMING SOON)")

        # elif isinstance(action, Duplication):
        #     history_text_output.append("Duplication of {}-{} (Query) {}-{} (Ref)".format(
        #         prtNum(int(action.start_x)),
        #         prtNum(int(action.start_x + action.length)),
        #         prtNum(int(action.start_y)),
        #         prtNum(int(action.start_y + action.height))
        #     ))

    with open(mkpath(output_folder, "history.txt"), 'w', encoding="utf-8") as file:
        file.write('\n\n'.join(history_text_output) + '\n')

    for action_index, action in enumerate(large_actions):

        if isinstance(action, Rotation):
            for line_index in range(action.start_line, action.end_line + 1):
                lines[line_index].rotateY(action.rotation_center, line=True, dots=True)

        elif isinstance(action, Insertion):
            for line in rotated_lines:

                if line.center_y >= action.start_y:
                    line.shift(dy=-action.height)

                # new_dots = []
                # for dot_x, dot_y in line.dots:
                #     if dot_y > action.start_y:
                #         dot_y -= action.height
                #     # if dot_x != action.start_x:
                #     #     new_dots.append([dot_x, dot_y])
                # line.dots = new_dots

            for i in range(action_index + 1, len(large_actions)):
                if isinstance(large_actions[i], Insertion):
                    large_actions[i].start_y -= action.height

        elif isinstance(action, Deletion):
            for line in rotated_lines:
                if line.center_x >= action.start_x:
                    line.shift(dx=-action.length)

                # for i in range(len(line.dots)):
                #     if line.dots[i][0] >= action.start_x + action.length:
                #         line.dots[i][0] -= action.length

            for i in range(action_index + 1, len(large_actions)):
                if isinstance(large_actions[i], Deletion):
                    large_actions[i].start_x -= action.length

        # elif isinstance(action, Duplication):
        #     new_dots = []
        #     for dot in rotated_lines[action.line_index].dots:
        #         if not (action.start_x <= dot[0] <= action.start_x + action.length):
        #             new_dots.append(dot)
        #     rotated_lines[action.line_index].dots = new_dots

        #     for line in rotated_lines:
        #         for i in range(len(line.dots)):
        #             if line.dots[i][0] >= action.start_x:
        #                 line.dots[i][1] -= action.height

        #     for i in range(action_index + 1, len(large_actions)):
        #         if large_actions[i].start_x >= action.start_x:
        #             large_actions[i].start_y -= action.height

        elif isinstance(action, Translocation):
            # tmp_rotated_lines = rotated_lines.copy()
            # for i in range(action.start_line, action.end_line + 1):
            #     rotated_lines[i] = tmp_rotated_lines[action.order[i - action.start_line]]

            # print("tmp_rotated_lines = ", *tmp_rotated_lines, sep='\n')
            # print("rotated_lines = ", *rotated_lines, sep='\n')

            # cur_x = min(rotated_lines[line_index].start_x for line_index in range(action.start_line, action.end_line + 1))
            cur_y = min(min(rotated_lines[line_index].start_y, rotated_lines[line_index].end_y) for line_index in range(action.start_line, action.end_line + 1))
            for line_index in range(action.start_line, action.end_line + 1):
                rotated_lines[line_index].shift(
                    # dx=cur_x - rotated_lines[line_index].start_x,
                    dy=cur_y - rotated_lines[line_index].start_y
                )
                # cur_x += rotated_lines[line_index].sizeX  # cur_x = rotated_lines[line_index].end_x
                cur_y += rotated_lines[line_index].sizeY  # cur_y = rotated_lines[line_index].end_y

        elif isinstance(action, Pass):
            pass

        else:
            raise ValueError("History: Unknown action type")

        if isinstance(action, (Pass, Rotation)):
            # plot.scatter(dots, dotsize=settings["dotsize"], color="#00f")
            for line in lines:
                plot.scatter(line.dots, dotsize=settings["dotsize"], color="#00f")
        else:
            # Adjusting axes (bottom):
            bottom = float("inf")
            for line in rotated_lines:
                for dot_x, dot_y in line.dots:
                    bottom = min(bottom, dot_y)
            if bottom == float('inf'):
                bottom = 0

            for line in rotated_lines:
                line.shift(dy=-bottom)

            for i in range(action_index + 1, len(large_actions)):
                if hasattr(large_actions[i], "start_x") and hasattr(large_actions[i], "start_y") and \
                        large_actions[i].start_x >= action.start_x:
                    large_actions[i].start_y += bottom

            # for line in rotated_lines:
            #     plot.plotLine(line)

            for line in rotated_lines:
                plot.scatter(line.dots, dotsize=settings["dotsize"], color="#00f")

        print("Saving large action #{}{}...\n".format(action_index, "" if isinstance(action, Pass) else " ({})".format(action.type)))
        plot.tight()
        plot.save(mkpath(
            output_folder,
            "history",
            "{}{}.png".format(
                str(action_index).zfill(3),
                "" if isinstance(action, Pass) else " ({})".format(action.type)
            )
        ))
        plot.clear()

    del plot


if __name__ == "__main__":
    removePythonCache(ROOT_PATH)
    analyze(query_genome_path, ref_genome_path, sam_file_path, show_plot, output_folder, SETTINGS)
    removePythonCache(ROOT_PATH)
