#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <cstdint>

struct Args {
    bool get_hist = false;
    bool get_r = true;
    bool get_n = true;
    int nseqs = 1;
    std::string pref = "out";
};

void scan_bwt(FILE* fp, Args args) {
    int c, pc = '\n';
    uint64_t r = 1;
    uint64_t n = 0;
    uint64_t l = 0;
    FILE* hist_fp;
    std::map<uint64_t, uint64_t> hist;
    if (args.get_hist) {
        std::string hist_fname = args.pref + ".hist";
        hist_fp = fopen(hist_fname.data(), "w");
    }
    // initialize loop
    c =  fgetc(fp);
    pc = c; ++n; ++l;
    while ((c = fgetc(fp)) != EOF) {
        if (pc != c) {
            ++r;
            if (args.get_hist) {
                if (hist.find(l) == hist.end()) {
                    hist[l] = 0;
                } hist[l] += 1 ;
            }
            l = 0;
        } 
        ++l;
        ++n;
        pc = c;
    }
    if (args.get_n) fprintf(stdout, "n: %llu\n", n);
    if (args.get_r) fprintf(stdout, "r: %llu\n", r);
    if (args.get_hist) {
        for (auto x: hist) {
            fprintf(hist_fp, "%llu\t%llu\n", x.first, x.second);
        }
        fclose(hist_fp);
        fprintf(stdout, "hist: see %s\n", (args.pref + ".hist").data());
    }
    return;
}

int main(int argc, char** argv) {
    int c;
    int long_index = 0;
    Args args;
    static struct option long_options[] {
        {"hist", no_argument, 0, 'h'},
        {"nruns", no_argument, 0, 'r'},
        {"length", no_argument, 0, 'n'},
        {"output_prefix", no_argument, 0, 'o'}
    };
    while((c = getopt_long(argc, argv, "hrn", long_options, &long_index)) != -1) {
        switch (c) {
            case 'h':
                args.get_hist = true;
                break;
            case 'r':
                args.get_r = true;
                break;
            case 'n':
                args.get_n = true;
                break;
            case 'o':
                args.pref = argv[optind];
                break;
            default:
                fprintf(stderr, "ignoring option %c\n", c);
                exit(1);
        }
    }
    if (argc - optind < 1) {
        fprintf(stderr, "no arguments given!\n");
        exit(1);
    }
    char* fname = argv[optind];
    FILE* fp = fopen(fname, "r");
    scan_bwt(fp, args);
    fclose(fp);
}
