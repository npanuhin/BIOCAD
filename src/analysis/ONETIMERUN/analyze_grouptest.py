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

    # for foldername in os.listdir(mkpath(ROOT_PATH, "output", "analysis")):
    #     if not os.path.isdir(mkpath(ROOT_PATH, "output", "analysis", foldername)) or foldername.strip("/").strip("\\") == "small":
    #         continue

    #     if int(foldername.strip("/").strip("\\").lstrip("large")) > 7:
    #         continue

    #     analysis.analyze(
    #         name=foldername,
    #         query_genome_path="samples/{}/large_genome1.fasta".format(foldername),
    #         ref_genome_path="samples/{}/large_genome2.fasta".format(foldername),
    #         # segments_file_path="BWA/{}/bwa_output.sam".format(foldername),
    #         segments_file_path="{}".format(analysis.segment_paths[foldername]),
    #         show_plot=False,
    #         output_folder="output/analysis/{}".format(foldername),
    #         settings=SETTINGS.copy()
    #     )

    for pair_name in os.listdir(mkpath(ROOT_PATH, "output", "alignment_grouptest")):
        first_genome_name, second_genome_name, _ = pair_name.split(".fasta_")
        # print(first_genome_name, second_genome_name)

        first_genome_path = mkpath("samples", "grouptest", first_genome_name + ".fasta")
        second_genome_path = mkpath("samples", "grouptest", second_genome_name + ".fasta")

        analysis.analyze(
            name=pair_name,
            query_genome_path=first_genome_path,
            ref_genome_path=second_genome_path,
            segments_file_path=mkpath("output", "alignment_grouptest", pair_name),
            show_plot=False,
            output_folder=mkpath("output", "analysis_grouptest", pair_name),
            settings=SETTINGS.copy()
        )


if __name__ == "__main__":
    removePythonCache()
    main()
    removePythonCache()
