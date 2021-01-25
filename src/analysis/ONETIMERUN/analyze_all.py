import os
from sys import path as sys_path

os.chdir("../")
sys_path.append("./")

import analysis
from utils import removePythonCache

ROOT_PATH = "../../"


def mkpath(*paths):
    return os.path.normpath(os.path.join(*paths))


def main():
    # Small
    SETTINGS = {
        "grid_size": 100,
        "min_block_size": 1,
        "dot_skip_rate": 1,
        "dotsize": 0.1,
        "fontsize": 10,
        "figsize": (10, 7),

        "min_event_size": 3,
        "lines_join_size": 5,
        "line_min_size": 10
    }

    # for foldername in os.listdir(mkpath(ROOT_PATH, "output", "analysis", "small")):
    #     analysis.analyze(
    #         name=foldername,
    #         query_genome_path="samples/small/source.fasta",
    #         ref_genome_path="samples/small/{}.fasta".format(foldername),
    #         segments_file_path="BWA/small/{}/bwa_output.sam".format(foldername),
    #         show_plot=False,
    #         output_folder="output/analysis/small/{}".format(foldername),
    #         settings=SETTINGS.copy()
    #     )

    # Large
    SETTINGS = {
        "grid_size": int(1e5),
        "min_block_size": int(1e3),
        "dot_skip_rate": 10,
        "dotsize": 0.1,
        "fontsize": 8,
        "figsize": (10, 7),

        "min_event_size": int(1e4),
        "lines_join_size": "$min_event_size + 3",
        "line_min_size": "$min_event_size"
    }

    for foldername in os.listdir(mkpath(ROOT_PATH, "output", "analysis")):
        if not os.path.isdir(mkpath(ROOT_PATH, "output", "analysis", foldername)) or foldername.strip("/").strip("\\") == "small":
            continue

        if int(foldername.strip("/").strip("\\").lstrip("large")) > 6:
            continue

        analysis.analyze(
            name=foldername,
            query_genome_path="samples/{}/large_genome1.fasta".format(foldername),
            ref_genome_path="samples/{}/large_genome2.fasta".format(foldername),
            # segments_file_path="BWA/{}/bwa_output.sam".format(foldername),
            segments_file_path="{}".format(analysis.segment_paths[foldername]),
            show_plot=False,
            output_folder="output/analysis/{}".format(foldername),
            settings=SETTINGS.copy()
        )


if __name__ == "__main__":
    removePythonCache()
    main()
    removePythonCache()
