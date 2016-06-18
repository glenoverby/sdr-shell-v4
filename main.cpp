#include <qapplication.h>
#include <qfontdatabase.h>
#include "main_widget.h"
#include "app.xpm"
#include <getopt.h>
#include <QSettings>


int main (int argc, char **argv)
{
    // Specify options and their syntax
    static struct option long_options[] =
    {
        /* These options set a flag. */
        {"verbose", no_argument, 0, 'v'},

        {"rx-command-port", required_argument, 0, 'r'},
        {"spectrum-port", required_argument, 0, 's'},
        {"meter-port", required_argument, 0, 'm'},
        {"tx-command-port", required_argument, 0, 't'},
        {"sample_rate", required_argument, 0, 'f'},
        {"host", required_argument, 0, 'h'},
        {"usbsoftrock-port", required_argument, 0, 'u'},
        {"conf-file", required_argument, 0, 'l'},
        {"help", no_argument, 0, '?'},
        {"?", no_argument, 0, '?'},
        {0, 0, 0, 0}
    };


    QApplication app(argc, argv);
    Main_Widget* w = new Main_Widget;

    // Iterate over options and handle each one as appropriate
    char c;
    // The function getopt_long stores the option index here.
    int option_index = 0;

    while ( (c = getopt_long ( argc, argv, "v?h:r:s:m:t:f:h:u:l:",
                               long_options, &option_index )) != -1 )
    {
        switch ( c )
        {
        case 0:
            // If this option set a flag, do nothing else now.
            if ( long_options[option_index].flag != 0 )
                break;
        case 'f':
            w->set_SampleRate(atoi(optarg));
            break;
        case 'h':
            w->set_Host(optarg);
            fprintf(stderr, "host is: %s",optarg);
            break;
        case 'm':
            w->set_MeterPort(atoi(optarg));
            break;
        case 'l':
            w->set_InitFile(optarg);
            break;
        case 'r':
            w->set_rxCMDPort(atoi(optarg));
            break;
        case 's':
            w->set_SpectrumPort(atoi(optarg));
            break;
        case 't':
            w->set_txCMDPort(atoi(optarg));
            break;
        case 'u':
            w->set_USBPort(atoi(optarg));
            break;
        case 'v':
            w->set_Verbose(true);
            break;
        case '?':

        default :
                printf(
                        "Options are: \n"
                        "-v or --verbose for verbose output\n"
                        "-f or --sample-rate=<samplerate>\n"
                        "-r or --rx-command-port=<portnum> \n"
                        "   Use port <portnum> as a conduit for update commands\n"
                        "   Default is 19001\n"
                        "-s or --spectrum-port=<portnum> \n"
                        "   Use port <portnum> as conduit for spectrum data\n"
                        "   Default is 19002\n"
                        "-m or --meter-port=<portnum> \n"
                        "   Use port <portnum> as conduit for meter data\n"
                        "   Default is 19003\n"
                        "-t or --tx-command-port=<portnum> \n"
                        "   Use port <portnum> as a conduit for update commands\n"
                        "   Default is 19005\n"
                        "-u or --usbsoftrock-port=<portnum> \n"
                        "   Use port <portnum> as a conduit for update commands\n"
                        "   Default is 19004\n"
                        "-l or --conf-file=<configuration file>\n"
                        "   The file is in ~/.config/freesoftware/ on Linux.\n"
                        "   \n"
                        );
        exit ( 0 );
        }
    }
    w->init();
    app.setWindowIcon( QIcon(app_xpm) );
    w->show();
    return app.exec();
}
