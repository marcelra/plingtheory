TEMPLATE = app
CONFIG += console
# CONFIG -= qt
QT += core gui
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
    UniformPdf.cpp \
    KernelPdf.cpp \
    IThread.cpp \
    IRealFunction.cpp \
    RealFunctionPtr.cpp \
    RealMemFunction.cpp \
    IRealFuncWithDerivative.cpp \
    ComposedRealFuncWithDerivative.cpp \
    NewtonSolver1D.cpp \
    LoggerClient.cpp \
    BisectionSolver1D.cpp \
    Interval.cpp \
    RootFinder1DBase.cpp \
    HistogramItem.cpp \
    McmcOptimiser.cpp \
    RandomNumberGenerator.cpp \
    MarkerDrawAttr.cpp \
    RootMlp.cpp \
    Hist2DItem.cpp \
    Regular2DHistogram.cpp \
    PcPixmapPaint.cpp \
    Palette.cpp \
    ParticleSwarmOptimiser.cpp \
    Hypercube.cpp \
    MlpTrainer.cpp \
    MultiLayerPerceptron.cpp \
    ZScatterItem.cpp \
    MlpErrorObjective.cpp \
    StochasticGradDescMlpTrainer.cpp \
    PredefinedRealFunctions.cpp \
    Chi2FitObjective.cpp \
    FitFunctionBase.cpp \
    PolynomialFitFunction.cpp \
    LineSearchGradDescOptimiser.cpp

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
    RealVector.h \
    LineSearchObjective.h \
    Utils.h \
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
    UniformPdf.h \
    KernelPdf.h \
    IThread.h \
    IRealFunction.h \
    RealFunctionPtr.h \
    RealMemFunction.h \
    IRealFuncWithDerivative.h \
    ComposedRealFuncWithDerivative.h \
    NewtonSolver1D.h \
    LoggerClient.h \
    BisectionSolver1D.h \
    Interval.h \
    RootFinder1DBase.h \
    HistogramItem.h \
    McmcOptimiser.h \
    RandomNumberGenerator.h \
    MarkerDrawAttr.h \
    RootMlp.h \
    Hist2DItem.h \
    Regular2DHistogram.h \
    PcPixmapPaint.h \
    Palette.h \
    ParticleSwarmOptimiser.h \
    Hypercube.h \
    MlpTrainer.h \
    MultiLayerPerceptron.h \
    ZScatterItem.h \
    MlpErrorObjective.h \
    GradDescOptimiser.h \
    StochasticGradDescMlpTrainer.h \
    PredefinedRealFunctions.h \
    Chi2FitObjective.h \
    FitFunctionBase.h \
    PolynomialFitFunction.h \
    LineSearchGradDescOptimiser.h

OTHER_FILES += \
    Todos.txt


QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
# QMAKE_MAC_SDK = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk
QMAKE_MAC_SDK=macosx

### BOOST
macx: LIBS += -L/usr/local/lib -lboost_filesystem -lboost_system -lboost_thread
linux: LIBS += -L/usr/local/lib -lboost_filesystem -lboost_system -lboost_thread

### ROOT
macx: LIBS += -L/usr/local/lib/root -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lz -Wl -lm -ldl -lMLP
macx: INCLUDEPATH += /usr/local/include/root
macx: INCLUDEPATH += /usr/local/include
linux: LIBS += -L/usr/local/lib/root -m64 -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lMLP -lCint -lTreePlayer
linux: INCLUDEPATH += /usr/local/include/root

### FFTW3
macx: LIBS += -L/usr/local/lib -lfftw3
linux: LIBS += -L/usr/local/lib -lfftw3

macx: QMAKE_CXXFLAGS += -g -ffast-math -O3
linux: QMAKE_CXXFLAGS += -g -ffast-math -mfpmath=387 -O3
# QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS += -std=c++11

