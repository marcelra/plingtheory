TEMPLATE = app
CONFIG += console
# CONFIG -= qt
QT += core  gui
CONFIG -= app_bundle
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp \
    RawPcmData.cpp \
    SamplingInfo.cpp \
    Logger.cpp \
    ProgramOptions.cpp \
    GlobalLogParameters.cpp \
    Msg.cpp \
    Exceptions.cpp \
    WaveFile.cpp \
    BinaryUtilities.cpp \
    SingletonStore.cpp \
    SingletonBase.cpp \
    RootUtilities.cpp \
    EffectTrianglizer.cpp \
    TestSuite.cpp \
    RollingBufferSawtoothTransform.cpp \
    IGenerator.cpp \
    GlobalParameters.cpp \
    Note.cpp \
    Duration.cpp \
    SineGenerator.cpp \
    MonophonicSimpleRandomMusicGenerator.cpp \
    SquareGenerator.cpp \
    MultiChannelRawPcmData.cpp \
    NoteList.cpp \
    SineEnvelopeGenerator.cpp \
    FftwAlgorithm.cpp \
    ObjectPool.cpp \
    NaivePeaks.cpp \
    Peak.cpp \
    Tone.cpp \
    FocalTones.cpp \
    IObjectiveFunction.cpp \
    TestMath.cpp \
    TwoDimExampleObjective.cpp \
    GradDescOptimiser.cpp \
    RealVector.cpp \
    LineSearchObjective.cpp \
    Utils.cpp \
    AlgorithmExceptions.cpp \
    WindowFunction.cpp \
    AlgorithmBase.cpp \
    StftGraph.cpp \
    WindowFuncDef.cpp \
    DynamicFourier.cpp \
    ResonanceMatrixVisualisation.cpp \
    StftAlgorithm.cpp \
    AdsrEnvelope.cpp \
    ISynthEnvelope.cpp \
    NoiseGenerator.cpp \
    TriangleGenerator.cpp \
    SawtoothGenerator.cpp \
    FourierSpectrum.cpp \
    FourierTransform.cpp \
    FourierConfig.cpp \
    DevSuite.cpp \
    SpectralReassignmentTransform.cpp \
    DevSuite.parked.cpp \
    TestDataSupply.cpp \
    TwoTuple.cpp \
    INTuple.cpp \
    SampledMovingAverage.cpp \
    WeightFunction.cpp \
    SortCache.cpp \
    RootFileCompare.cpp \
    IAccumArray.cpp \
    RegularAccumArray.cpp \
    RebinnedSRGraph.cpp \
    GroundtoneHypothesisBuilder.cpp \
    AccumArrayPeakAlgorithm.cpp \
    StftData.cpp \
    SrSpectrum.cpp \
    MainWindow.cpp \
    DevGui.cpp \
    AvailablePlotsList.cpp \
    Plot2D.cpp \
    IPaintItem.cpp \
    AxisPaintArea.cpp \
    CurveItem.cpp \
    GridItem.cpp \
    HorizontalScrollPaintArea.cpp \
    IPaintCommand.cpp \
    ITwoDimPlotData.cpp \
    IYVsXItem.cpp \
    LineDrawAttr.cpp \
    PaintArea.cpp \
    PaintAreaBase.cpp \
    ScrollPaintArea.cpp \
    VerticalScrollPaintArea.cpp \
    XAxisPaintArea.cpp \
    YAxisPaintArea.cpp \
    YVsXData.cpp \
    PcLinePaint.cpp \
    PcSetDrawAttr.cpp \
    PcRectanglePaint.cpp \
    RegLargeDataCurve.cpp \
    PcMarkerPaint.cpp \
    ScatterItem.cpp \
    IPlotFactory.cpp \
    RootPlotFactory.cpp \
    QtPlotFactory.cpp \
    StftPaintItem.cpp \
    IPdf.cpp \
    GaussPdf.cpp \
    UniformPdf.cpp

HEADERS += \
    RawPcmData.h \
    SamplingInfo.h \
    Logger.h \
    ProgramOptions.h \
    GlobalLogParameters.h \
    Msg.h \
    Exceptions.h \
    WaveFile.h \
    BinaryUtilities.h \
    SingletonStore.h \
    SingletonBase.h \
    RootUtilities.h \
    EffectTrianglizer.h \
    TestSuite.h \
    RollingBufferSawtoothTransform.h \
    IGenerator.h \
    GlobalParameters.h \
    Note.h \
    Duration.h \
    SineGenerator.h \
    MonophonicSimpleRandomMusicGenerator.h \
    SquareGenerator.h \
    MultiChannelRawPcmData.h \
    NoteList.h \
    FourierNoteFilter.h \
    SineEnvelopeGenerator.h \
    FftwAlgorithm.h \
    ObjectPool.h \
    NaivePeaks.h \
    Peak.h \
    Tone.h \
    Typedefs.h \
    FocalTones.h \
    IObjectiveFunction.h \
    TestMath.h \
    TwoDimExampleObjective.h \
    GradDescOptimiser.h \
    RealVector.h \
    LineSearchObjective.h \
    Utils.h \
    AlgorithmExceptions.h \
    WindowFunction.h \
    AlgorithmBase.h \
    IStorable.h \
    StftGraph.h \
    WindowFuncDef.h \
    DynamicFourier.h \
    ResonanceMatrixVisualisation.h \
    StftAlgorithm.h \
    AdsrEnvelope.h \
    ISynthEnvelope.h \
    NoiseGenerator.h \
    TriangleGenerator.h \
    SawtoothGenerator.h \
    FourierSpectrum.h \
    FourierTransform.h \
    FourierConfig.h \
    DevSuite.h \
    SpectralReassignmentTransform.h \
    TestDataSupply.h \
    TwoTuple.h \
    INTuple.h \
    SampledMovingAverage.h \
    WeightFunction.h \
    SortCache.h \
    RootFileCompare.h \
    IAccumArray.h \
    RegularAccumArray.h \
    RebinnedSRGraph.h \
    GroundtoneHypothesisBuilder.h \
    AccumArrayPeakAlgorithm.h \
    StftData.h \
    SrSpectrum.h \
    MainWindow.h \
    DevGui.h \
    AvailablePlotsList.h \
    Plot2D.h \
    IPaintItem.h \
    AxisPaintArea.h \
    CurveItem.h \
    GridItem.h \
    HorizontalScrollPaintArea.h \
    IPaintCommand.h \
    ITwoDimPlotData.h \
    IYVsXItem.h \
    LineDrawAttr.h \
    PaintArea.h \
    PaintAreaBase.h \
    ScrollPaintArea.h \
    VerticalScrollPaintArea.h \
    XAxisPaintArea.h \
    YAxisPaintArea.h \
    YVsXData.h \
    PcLinePaint.h \
    PcSetDrawAttr.h \
    PcRectanglePaint.h \
    RegLargeDataCurve.h \
    PcMarkerPaint.h \
    ScatterItem.h \
    IPlotFactory.h \
    RootPlotFactory.h \
    QtPlotFactory.h \
    StftPaintItem.h \
    IPdf.h \
    GaussPdf.h \
    UniformPdf.h

OTHER_FILES += \
    Todos.txt


### BOOST
macx: LIBS += -L/usr/local/lib/devTools/boost_1_50_0/lib -lboost_filesystem -lboost_system
linux: LIBS += -L/usr/local/lib/ -lboost_filesystem -lboost_system
#INCLUDEPATH += /Users/marcelra/devTools/boost_1_50_0/
#DEPENDPATH += /Users/marcelra/devTools/boost_1_50_0/stage

### ROOT
macx: LIBS += -L/usr/local/lib/root -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lz -pthread -Wl -lm -ldl
linux: LIBS += -L/usr/root/lib/ -m64 -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic
macx: INCLUDEPATH += /usr/local/include/root
linux: INCLUDEPATH += /usr/root/include
DEPENDPATH += /usr/root/include

### FFTW3
macx: LIBS += -L/usr/local/lib/ -lfftw3
linux: LIBS += -L/usr/local/lib/ -lfftw3

QMAKE_CXXFLAGS += -g -O3 -ffast-math -mfpmath=387

# macx: PRE_TARGETDEPS += $$PWD/usr/root/lib/libAfterImage.a

