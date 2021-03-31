/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* options.cpp */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "options.h"

im2a::Options::Options(int argc, char *const *argv)
{
    _name = argv[0];

    /* defaults */
    _html = false;
    _invert = false;
    _center = false;
    _pixel = false;
    _width = 0;
    _height = 0;
    _charset = NULL;
    _grayscale = false;
    _transparent = false;
    _transparency_threshold = 1.0000f;
    _red_weight = 0.2989f;
    _green_weight = 0.5866f;
    _blue_weight = 0.1145f;
    _source = NULL;

    int ch;
    const struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"html", no_argument, NULL, 'm'},
        {"invert", no_argument, NULL, 'i'},
        {"center", no_argument, NULL, 't'},
        {"pixel", no_argument, NULL, 'p'},
        {"width", required_argument, NULL, 'W'},
        {"height", required_argument, NULL, 'H'},
        {"charset", required_argument, NULL, 'c'},
        {"grayscale", no_argument, NULL, 'g'},
        {"transparent", no_argument, NULL, 'T'},
        {"transparency-threshold", required_argument, NULL, 'X'},
        {"red-weight", required_argument, NULL, 'R'},
        {"green-weight", required_argument, NULL, 'G'},
        {"blue-weight", required_argument, NULL, 'B'},
        {NULL, 0, NULL, 0}
    };

    /* parse comment line options */
    while ((ch = getopt_long(argc, argv, "hvmitpW:H:c:gTX:R:G:B:", longopts,
        NULL)) != -1) {
        switch (ch) {
            case 'h':
                usage(std::cout, 0);
                break;

            case 'v':
                std::cout << PROJECT_VERSION << std::endl;
                exit(EXIT_SUCCESS);

            case 'm':
                _html = true;
                break;

            case 'i':
                _invert = true;
                break;

            case 't':
                _center = true;
                break;

            case 'p':
                _pixel = true;
                break;

            case 'W':
                _width = atoi(optarg);
                if (_width <= 0) {
                    std::cerr << _name
                        << ": invalid width: " << _width << std::endl;
                    std::cerr << std::endl;
                    usage();
                }
                break;

            case 'H':
                _height = atoi(optarg);
                if (_height <= 0) {
                    std::cerr << _name <<
                        ": invalid height: " << _height << std::endl;
                    std::cerr << std::endl;
                    usage();
                }
                break;

            case 'c':
                _charset = strdup(optarg);
                if (strlen(_charset) < 2) {
                    std::cerr << _name <<
                        ": charset should be at least 2 symbols long" <<
                        std::endl;
                    std::cerr << std::endl;
                    usage();
                }
                break;

            case 'g':
                _grayscale = true;
                break;

            case 'T':
                _transparent = true;
                break;

            case 'X':
                _transparency_threshold = atof(optarg);
                break;

            case 'R':
                _red_weight = atof(optarg);
                break;

            case 'G':
                _green_weight = atof(optarg);
                break;

            case 'B':
                _blue_weight = atof(optarg);
                break;

            default:
                std::cerr << std::endl;
                usage();
        }
    }

    /* check for options compatibility */
    if (_html && _pixel) {
        std::cerr << _name <<
            ": cannot use --html and --pixel at the same time" << std::endl;
        std::cerr << std::endl;
        usage();
    }

    /* if no charset provided, use default */
    if (!_charset) {
        _charset = strdup(" M   ...',;:clodxkO0KXNWMM");
    }

    /* check for 1 positional argument */
    if (argc - optind != 1) {
        std::cerr << _name <<
            ": wrong number of arguments " <<
            "(given " << argc - 1 << ", expected " << 1 << ")" <<
            std::endl;
        std::cerr << std::endl;
        usage();
    }

    _source = strdup(argv[optind]);
}

im2a::Options::~Options()
{
    if (_charset) {
        free(_charset);
    }
    if (_source) {
        free(_source);
    }
}

void im2a::Options::usage(std::ostream &out, int exit_code)
{
    out << PROJECT_NAME << " " PROJECT_VERSION << std::endl;
    out << std::endl;

    out << "Convert image files to ASCII art" << std::endl;
    out << "Copyright (C) 2013 Latchezar Tzvetkoff" << std::endl;
    out << "Distributed under The Beerware License" << std::endl;
    out << std::endl;

    out << "Usage:" << std::endl;
    out << "  " << _name << " [options] <file>" << std::endl;
    out << std::endl;

    out << "Common options:" << std::endl;
    out << "  -h, --help                        Prints this message" <<
        std::endl;
    out << "  -v, --version                     Prints version string" <<
        std::endl;
    out << std::endl;

    out << "Specific options:" << std::endl;
    out << "  -m, --html                        Output HTML" << std::endl;
    out << "  -i, --invert                      Invert the image" << std::endl;
    out << "  -t, --center                      Center the image" << std::endl;
    out << "  -p, --pixel                       Pixel mode" << std::endl;
    out << "  -W, --width=N                     Set output width" << std::endl;
    out << "  -H, --height=M                    Set output height" <<
        std::endl;
    out << "  -c, --charset=CHARSET             Set output charset" <<
        std::endl;
    out << "  -g, --grayscale                   Grayscale output instead"
        " of 256-color" << std::endl;
    out << "  -T, --transparent                 Enable transparency" <<
        std::endl;
    out << "  -X, --transparency-threshold=X    Set transparency threshold"
        " (default: 1.0)" << std::endl;
    out << "  -R, --red-weight=RW               Set red component weight"
        " (default: 0.2989)" << std::endl;
    out << "  -G, --green-weight=GW             Set green component weight"
        " (default: 0.5866)" << std::endl;
    out << "  -B, --blue-weight=BW              Set blue component weight"
        " (default: 0.1145)" << std::endl;
    out << std::endl;

    out << "Project homepage: " PROJECT_HOMEPAGE_URL << std::endl;
    out << "Report bugs to: " PROJECT_BUGREPORT_URL << std::endl;

    exit(exit_code);
}

bool im2a::Options::html() const
{
    return _html;
}

bool im2a::Options::invert() const
{
    return _invert;
}

bool im2a::Options::center() const
{
    return _center;
}

bool im2a::Options::pixel() const {
    return _pixel;
}

int im2a::Options::width() const
{
    return _width;
}

int im2a::Options::height() const
{
    return _height;
}

char *im2a::Options::charset() const
{
    return _charset;
}

bool im2a::Options::grayscale() const
{
    return _grayscale;
}

bool im2a::Options::transparent() const
{
    return _transparent;
}

double im2a::Options::transparency_threshold() const
{
    return _transparency_threshold;
}

double im2a::Options::red_weight() const
{
    return _red_weight;
}

double im2a::Options::green_weight() const
{
    return _green_weight;
}

double im2a::Options::blue_weight() const
{
    return _blue_weight;
}

char *im2a::Options::source() const
{
    return _source;
}
