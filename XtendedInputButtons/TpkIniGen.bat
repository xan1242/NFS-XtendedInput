@echo off
:: TPK Generator for XNFSTPKTool
set ININAME=8A69054A.ini
IF NOT "%~1"=="" set ININAME="%1"

<nul (set/p nlv=[TPK])> %ININAME%                              & ECHO(>> %ININAME%
<nul (set/p nlv=Name = XTENDEDINPUTBUTTONS)>> %ININAME%        & ECHO(>> %ININAME%
<nul (set/p nlv=Version = 4)>> %ININAME%                       & ECHO(>> %ININAME%
<nul (set/p nlv=Filename = XtendedInputButtons)>> %ININAME%    & ECHO(>> %ININAME%
<nul (set/p nlv=FilenameHash = 0x)>> %ININAME%
strhash XtendedInputButtons>> %ININAME%
ECHO(>> %ININAME%
<nul (set/p nlv=Animations = 0)>> %ININAME%
ECHO(>> %ININAME%
ECHO(>> %ININAME%
FOR /R . %%T IN (*.dds) DO (
<nul (set/p nlv=[)>> %ININAME%
strhash %%~nT>> %ININAME%
<nul (set/p nlv=])>> %ININAME%                                 & ECHO(>> %ININAME%
<nul (set/p nlv=File = %%~dpnT.dds)>> %ININAME%                & ECHO(>> %ININAME%
<nul (set/p nlv=Name = %%~nT)>> %ININAME%                      & ECHO(>> %ININAME%
<nul (set/p nlv=ClassNameHash = 0x1A93CF)>> %ININAME%          & ECHO(>> %ININAME%
<nul (set/p nlv=ImageCompressionType = 0x24)>> %ININAME%       & ECHO(>> %ININAME%
<nul (set/p nlv=PaletteCompressionType = 0x0)>> %ININAME%      & ECHO(>> %ININAME%
<nul (set/p nlv=NumPaletteEntries = 0)>> %ININAME%             & ECHO(>> %ININAME%
<nul (set/p nlv=TilableUV = 0)>> %ININAME%                     & ECHO(>> %ININAME%
<nul (set/p nlv=BiasLevel = 0)>> %ININAME%                     & ECHO(>> %ININAME%
<nul (set/p nlv=RenderingOrder = 5)>> %ININAME%                & ECHO(>> %ININAME%
<nul (set/p nlv=ScrollType = 0)>> %ININAME%                    & ECHO(>> %ININAME%
<nul (set/p nlv=UsedFlag = 0)>> %ININAME%                      & ECHO(>> %ININAME%
<nul (set/p nlv=ApplyAlphaSorting = 0)>> %ININAME%             & ECHO(>> %ININAME%
<nul (set/p nlv=AlphaUsageType = 2)>> %ININAME%                & ECHO(>> %ININAME%
<nul (set/p nlv=AlphaBlendType = 1)>> %ININAME%                & ECHO(>> %ININAME%
<nul (set/p nlv=Flags = 0x0)>> %ININAME%                       & ECHO(>> %ININAME%
<nul (set/p nlv=ScrollTimeStep = 0)>> %ININAME%                & ECHO(>> %ININAME%
<nul (set/p nlv=ScrollSpeedS = 0)>> %ININAME%                  & ECHO(>> %ININAME%
<nul (set/p nlv=ScrollSpeedT = 0)>> %ININAME%                  & ECHO(>> %ININAME%
<nul (set/p nlv=OffsetS = 0)>> %ININAME%                       & ECHO(>> %ININAME%
<nul (set/p nlv=OffsetT = 256)>> %ININAME%                     & ECHO(>> %ININAME%
<nul (set/p nlv=ScaleS = 256)>> %ININAME%                      & ECHO(>> %ININAME%
<nul (set/p nlv=ScaleT = 0)>> %ININAME%                        & ECHO(>> %ININAME%
<nul (set/p nlv=PixelFormatUnk1 = 0x1)>> %ININAME%             & ECHO(>> %ININAME%
<nul (set/p nlv=PixelFormatUnk2 = 0x5)>> %ININAME%             & ECHO(>> %ININAME%
<nul (set/p nlv=PixelFormatUnk3 = 0x6)>> %ININAME%             & ECHO(>> %ININAME%
ECHO(>> %ININAME%
)
EXIT /B 0
