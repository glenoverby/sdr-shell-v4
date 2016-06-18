#ifndef SDXCVR_MAINWIDGET_H
#define SDXCVR_MAINWIDGET_H

#define	VERSION 4.25

#include <stdlib.h>
#include <qwidget.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qfont.h>
#include <qpixmap.h>
#include <qlistview.h>
#include <QLabel>
#include <qslider.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qfile.h>
#include <QSettings>
#include <qpainter.h>
#include <QPaintDevice>
#include <qimage.h>
#include <qlcdnumber.h>
#include <qdir.h>
#include <QTextEdit>
#include <QTextOption>
#include <QIODevice>
#include <QDataStream>
#include <qscrollbar.h>
#include <qtabwidget.h>
#include <qspinbox.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qmutex.h>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPalette>
#include <QString>
#include <QVBoxLayout>
#include <QComboBox>

#include <time.h>
#include <sys/timeb.h>

#include "spectrogram.h"
#include "spectrum.h"
#include "varilabel.h"
#include "memorycell.h"
#include "pbscale.h"
#include "hamlibwrapper.h"
#include "rigctl.h"
#include "dttsp.h"
#include "freqlabel.h"
#include "lcdfreq.h"
#include "command.h"
#include "idbutton.h"

#include "cmath"

// DttSP constants
#define DEFSPEC (4096)
#define MAXRX (4)
#define RXMETERPTS (5)
#define TXMETERPTS (9)

#define SPECTRUM_HISTORY_SIZE (1000)

#define ONE_OVER_F_GUARD_FREQUENCY (2000)  /*This is for the usbsoftrock which uses 
only the upper half of the spectrum in order to omit the 1/f noise near DC */

#define NUM_MODES 12

#define NUM_BANDMEMS 10
#define NUM_MEMS 8
#define	NUM_CMD	8

#define TOPFRM_V 35
#define PBSFRM_V 15
#define SPECFRM_V 120

const int FREQUENCY_UPDATE = 2;
const int OFFSET = 10000;

class Main_Widget : public QWidget
{
		Q_OBJECT

	private:
                //Argument variables from main//
                int rxCMDPort;
                int txCMDPort;
                int usbPort;
                int meterPort;
                int spectrumPort;
                char *host;
                bool verbose;
                QSettings *settings;

                //QPushButton *quit_button;
		QPushButton *updateLOFreqButton;
		QPushButton *updateTuneOffsetButton;
		QPushButton *cmd0reset;
		//QScrollView *scroll_view;
		QFont *font;
		QLineEdit *cfgCallInput, *cfgLOFreqInput, *cfgIFreqInput, *cfgHamlibRigInput, *cfgHamlibSpeedInput, *cfgHamlibPortInput;
		QLineEdit *cfgTuneOffsetInput;
		QSpinBox *cfgIQPhaseInput, *cfgIQGainInput, *cfgUSBOffsetInput;
		QSpinBox *cfgTxIQPhaseInput, *cfgTxIQGainInput;
		QSpinBox *cfgTxMicGainInput, *cfgTxOutputGainInput;
		QSpinBox *specCalSpinBox, *metrCalSpinBox, *cfgLSBOffsetInput;
		QSpinBox *cfgSlopeLowOffsetInput, *cfgSlopeHighOffsetInput;
        QSpinBox *NR_TapsSpinBox, *NR_DelaySpinBox; 
        QDoubleSpinBox *NR_GainSpinBox, *NR_LeakageSpinBox; 
        QSpinBox *ANF_TapsSpinBox, *ANF_DelaySpinBox; 
        QDoubleSpinBox *ANF_GainSpinBox, *ANF_LeakageSpinBox; 
        QDoubleSpinBox *NB_ThresholdSpinBox;
		QSpinBox *cfgSpecAvgInput, *cfgSpecLowInput, *cfgSpecHighInput;
 
		QPixmap *rxPix, *txPix;
		QFrame *trxFrame;
        hamlibWrapper *ourHamlibWrapper;
		RigCtlServer *rigCtl;
		QMutex displayMutex, update_freqMutex;

        //hamlib parameters.
		QString portString;
		rig_model_t rig;
		QString rigString;
		int speed;
		QString speedString;
		bool useHamlib;
        void initHamlib ();
		void initRigCtl();
		bool useSlopeTune;
		bool muteXmit;

		bool useIF;
		int usbOffset;
		int lsbOffset;
		int slopeLowOffset;
		int slopeHighOffset;
		int slopeTuneOffset;
		int cwPitch;
		QLabel *cfgSlopeLowOffsetLabel;
		QLabel *cfgSlopeHighOffsetLabel;
		bool rock_bound;	// True if Crystal, False USBSoftrock
		bool enableTransmit;
		bool dualConversion;
		bool enableRIT;
		bool enableSPLIT;

		Varilabel *TRX_label;
		Varilabel *NR_label;
		Varilabel *ANF_label;
		Varilabel *NB_label;
		Varilabel *BIN_label;
		Varilabel *MUTE_label;
		Varilabel *SPEC_label;
		Varilabel *UP_label;
		Varilabel *DOWN_label;
		Varilabel *RIT_label;
		Varilabel *SPLIT_label;
		QLabel *RX_label;
		QLabel *MHz_label;

		VariModelabel *LSB_label;
		VariModelabel *USB_label;
		VariModelabel *DSB_label;
		VariModelabel *AM_label;
		VariModelabel *CWL_label;
		VariModelabel *CWU_label;
		VariModelabel *SAM_label;
		VariModelabel *FMN_label;
		Varilabel *CFG_label;
		Varilabel *HELP_label;
		Varilabel *AGC_O_label, *AGC_L_label, *AGC_S_label, *AGC_M_label, *AGC_F_label;
		Varilabel *Zoom_in_label, *Zoom_out_label;

		Freqlabel *THOUSANDS_label, *HUNDREDS_label, *TENS_label, *UNITS_label;

		PassBandScale *pbscale;

		QFont *font1;
		QFont *font2;
		QFont *fontlcd;
		QFontMetrics *font1Metrics;
		QFontMetrics *font2Metrics;
		QFontMetrics *fontlcdMetrics;
		QLabel *signal_S;
		QLabel *signal_dBm;
		QLabel *qtrLabel;
		QLabel *CA_label;
		QLabel *iqGainLabel, *iqPhaseLabel;
		QLabel *callsignLabel;
		QLabel *cmdLabel;
		QLabel *CPU_label;
		QLabel *f_label;
		QLabel *logoLabel;
		QLabel *M_label;
		QLabel *AGC_label;
		QLabel *Zoom_label;
		QLabel *Spacer_label;

		QFrame *logoFrame;

		MemoryCell *f_cell[NUM_MEMS];

		MemoryCell *band_cell[NUM_BANDMEMS];

		Command		*c_cell[NUM_CMD];
		QLineEdit	*cmdName[NUM_CMD], *cmdOnCommand[NUM_CMD];
		QLineEdit	*cmdOffCommand[NUM_CMD];
		QRadioButton *cmdRXbutton[NUM_CMD], *cmdTXbutton[NUM_CMD];
		//IdPushButton *cmd0accept; //, *cmd0reset;

		QLabel *af1_label;
		QLabel *af2_label;
		QLabel *af3_label;
		QLabel *af4_label;
		QLabel *af5_label;
		QLabel *af6_label;
		QLabel *af7_label;
		QLabel *af8_label;
		
		QLabel *cfgRigLabel;
		QLabel *cfgSpeedLabel;
		QLabel *cfgPortLabel;

		Spectrogram *spectrogram;
		Spectrum *spectrumFrame;
		QFrame *spectrogramFrame;
		QFrame *spectrumScale;
		QPixmap *spectrumPix;

		QFrame *ctlFrame;
		QFrame *ctlFrame2;
		QFrame *cfgFrame;
		QFrame *cmdFrame;
		QFrame *helpFrame;
		QFrame *dspFrame;
		QFrame *signalFrame;
		QFrame *signalBargraph[34];
		QFrame *map;
		QFrame *filterFrame;
		QColor *signalColor[34];
		QString *modeName[NUM_MODES];
		QString stationCallsign;
		QString stationQTH;
		//QLCDNumber *lcd;
		LCDFreq *lcd;
		QLabel *rit;
		QTextEdit *textFrame;
        QRadioButton *polyFFT_button, *preFilter_button, *postFilter_button,
			*fftWindow_0, *fftWindow_1, *fftWindow_2, *fftWindow_3, *fftWindow_4, *fftWindow_5,
			*fftWindow_6, *fftWindow_7, *fftWindow_8, *fftWindow_9,*fftWindow_10, *fftWindow_11,
			*fftWindow_12;
		QRadioButton *specLineFillButton;
		QRadioButton *cfgUseUSBsoftrock;
		QRadioButton *cfgDualConversion;

		QPalette p;

		QFrame *step_1Hz_frame;
		QFrame *step_10Hz_frame;
		QFrame *step_100Hz_frame;
		QFrame *step_1KHz_frame;
		QFrame *step_10KHz_frame;
		QFrame *step_100KHz_frame;
		QFrame *step_1MHz_frame;
		QFrame *step_10MHz_frame;
		QFrame *step_100MHz_frame;
		
		unsigned long long int rx_f, rx_if;
		unsigned long long int tx_f;
		QString rx_f_string, rx_if_string;
		QString tx_f_string;
		QString	TuneOffset_string;
		bool specLineFill;	// true=enables spectrum display line filled
                int sample_rate;
		int tuneCenter;		// where to put tune center when using usbsoftrock
		int spec_width;		// spectrum display width
		int rx_delta_f;		// relative to center frequeny
		int tx_delta_f;		// relative to center frequeny
		int tuneStep;
		int *filter_l, *filter_h, filter_w;
		int USB_filter_l, USB_filter_h;
		int LSB_filter_l, LSB_filter_h;
		int DSB_filter_l, DSB_filter_h;
		int CWL_filter_l, CWL_filter_h;
		int CWU_filter_l, CWU_filter_h;
		int SAM_filter_l, SAM_filter_h;
		int FMN_filter_l, FMN_filter_h;
		int AM_filter_l, AM_filter_h;
		int s_dbm[34];
		int spec_r[120], spec_g[120], spec_b[120];
		rmode_t mode;
		int iqGain, iqPhase;
		int txIQGain, txIQPhase, txGain, micGain;
		int NR_Taps, NR_Delay;
        float NR_Gain, NR_Leakage;
		int ANF_Taps, ANF_Delay;
        float ANF_Gain, ANF_Leakage;
		float NB_Threshold;
        int NR_state;
		int ANF_state;
		int NB_state;
		int BIN_state;
		int MUTE_state;
		int SPEC_state;
		int filterLine;
		int font1PointSize;
		int font2PointSize;
		int fontlcdPointSize;
		int theme;
		int map_flag;
        int polyphaseFFT;
		int fftWindow;
		int spectrumType;
		int agcType;
		int transmit;
		int band;
		int updated;
		char *sdr_mode;		// mode change script
		char *sdr_band;		// band change script
		char *sdr_rxtx;		// rx/tx change script

		int once;

		float spectrum[DEFSPEC];
		float oscope[DEFSPEC];
		float specApertureLow, specApertureHigh;
		float specCal;
		float metrCal;
		float hScale;
		float vsScale;		// vertical Spectrum scale
		float bin_bw;
		int specAveraging;		// number of spectrum data points to average on each display
		int specLow, specHigh;

		int spectrum_history[SPECTRUM_HISTORY_SIZE][DEFSPEC];
		int spectrum_head;
		int spectrum_width;

		float loadavg;
		double my_lon, my_lat;

		DttSPcmd      *pCmd;
		DttSPspectrum *pSpectrum;
		DttSPmeter    *pMeter;
		USBSoftrockCmd *pUSBCmd;
		DttSPTXcmd    *pTXCmd;

		FILE *fftFile;

		FILE *loadavg_stream;

		enum kb_state { RX_F, TX_F, FILTER_L, FILTER_H };
		enum modes
		{ LSB, USB, DSB, CWL, CWU, FMN, AM, DIGU, SPEC, DIGL, SAM, DRM }; 
		enum spec_type { SEMI_RAW, PRE_FILT, POST_FILT };
		enum radio_state { RX, TX };

		typedef enum _windowtype {
		    RECTANGULAR_WINDOW,
		    HANNING_WINDOW,
		    WELCH_WINDOW,
		    PARZEN_WINDOW,
		    BARTLETT_WINDOW,
		    HAMMING_WINDOW,
		    BLACKMAN2_WINDOW,
		    BLACKMAN3_WINDOW,
		    BLACKMAN4_WINDOW,
		    EXPONENTIAL_WINDOW,
		    RIEMANN_WINDOW,
		    BLACKMANHARRIS_WINDOW,
		    NUTTALL_WINDOW,
		} Windowtype;

		void initConstants();
		void rx_cmd ( int );
		void process_key ( int );
		void setRxFrequency( int );
		void setTxFrequency();
		void setDefaultRxFrequency();
                void loadSettings();
                void setupSDR();
                void saveSettings();
		void setScrollBarColors ( QScrollBar * );
		void setCA_label();
		void set_NR ( int );
		void set_ANF ( int );
		void set_NB ( int );
		void set_BIN ( int );
		void set_SPEC ( int );
		void set_RIT ( int );
		void set_SPLIT ( int );
		void setIQGain();
		void setIQPhase();
		void setTxIQGain();
		void setTxIQPhase();
		void setTxGain( int );
		void drawSpectrogram();
		void drawSpectrogram_2();
		void drawSpectrumScale();
		void drawSpectrumScale_2();
		void drawPassBandScale();
		void setTheme ( int );
		void updateLayout();
		void loadMemoryCells();
		void loadCommandCells();

	public:
                Main_Widget();
                void init();
                void set_rxCMDPort( int );
                void set_txCMDPort( int );
                void set_USBPort( int );
                void set_SpectrumPort( int );
                void set_MeterPort( int );
                void set_SampleRate( int );
                void set_Host( char * );
                void set_Verbose( bool );
                void set_InitFile(char *);

		rmode_t rigGetMode();
		int rigGetFilterWidth();
                unsigned long long int rigGetFrequency();
                void rigSetFrequency(unsigned long long int rigctlfreq);
                void rigSetPTT ( int );

	public slots:
		void finish();
		void readMeter();
		void readSpectrum();
		void spectrogramClicked ( int );
		void plotSpectrum ( int );
		void tune ( int );
		void tunef ( int );
		void tunewheel ( int );
		void processorLoad();

		void setTuneStep ( int );
		void toggle_NR ( int );
		void toggle_ANF ( int );
		void toggle_NB ( int );
		void toggle_BIN ( int );
		void toggle_MUTE ( int );
		void toggle_SPEC ( int );
		void leave_band ( int );
		void enter_band ( int );
		void band_UP ( int );
		void band_DOWN ( int );
		void toggle_TX ( int );
		void TXon();
		void TXoff();
		void toggle_RIT ( int );
		void toggle_SPLIT ( int );

		void setFilter_l ( int );
		void setFilter_h ( int );
		void setMode ( rmode_t, bool, bool );
		void setFilter();
		void f_at_mousepointer ( int );
		void setLowerFilterScale ( int );
		void setUpperFilterScale ( int );
		void setCfg ( int );
		void setHelp ( int );
		void setDSP ( int );

		void readMem ( MemoryCell * );
		void writeMem ( MemoryCell * );
		void displayMem ( MemoryCell * );
		void toggleCmd ( Command *);
		void configCmd ( Command * );
		void displayNCO ( int );

		void updateCallsign();
		void updateLOFreq();
		void updateIFreq();
		void updateUSBOffset ( int );
		void updateLSBOffset ( int );
		void updateSlopeHighOffset ( int offset );
		void updateSlopeLowOffset ( int offset );	
		void updateHamlib();
		void updateIQGain ( int );
		void updateIQPhase ( int );
		void updateTxIQGain ( int );
		void updateTxIQPhase ( int );
		void updateTxMicGain ( int );
		void updateTxOutputGain ( int );
                void setPolyFFT ( );
		void setFFTWindow ( );
		void updateSpecAvg ( int );
		void updateSpecLow ( int );
		void updateSpecHigh ( int );
		void setLineFill ( );
		void setSpectrumType ( );
		void setSpectrumDefaults ( );
		void setAGC ( int );
		void zoomIN ( int );
		void zoomOUT ( int );
		void calibrateSpec ( int );
		void calibrateMetr ( int );
        void setNR_Taps ( int );
        void setNR_Delay ( int );
        void setNR_Gain ( double);
        void setNR_Leakage ( double);
        void set_NRvals ( );
        void setANF_Taps ( int );
        void setANF_Delay ( int );
        void setANF_Gain ( double );
        void setANF_Leakage ( double );
        void set_ANFvals ( );
        void setNB_Threshold ( double );
        void set_NBvals ( );
		void updateUseUSBsoftrock ( bool );
		void updateTransmit ( bool );
		void updateDualConversion ( bool );
		void updateTuneOffset ( );
		void updateCmd (int n);
		void resetCmd (int n);
		void updatePTT();
		void updateFreq();
		
		void set_MUTE ( int );
		void setOurRxFrequency ( double );
		void setIF ( bool );
		void setHamlib ( bool );
		void setSlopeLowOffset ( int );
		void setSlopeHighOffset (int );
		void setSlopeTune ( bool );
		void setCWPitch ( int );
		void setMuteXmit ( bool );

	protected:
		void keyPressEvent ( QKeyEvent * );
		void paintEvent ( QPaintEvent * );
		void focusInEvent ( QFocusEvent * );
		void focusOutEvent ( QFocusEvent * );
		void closeEvent ( QCloseEvent * );
		
	signals:
		void changeRigMode (rmode_t, pbwidth_t );
		void changeSlopeTune ( bool );
		void toggleHamlibButton ( bool );
		void tellMuteXmit ( bool );
};
#endif
