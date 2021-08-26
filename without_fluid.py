# parameters
file_name = ''
array_name = ''
colorbar_title = ''

# open the config file to read parameters
with open("config.txt", 'r') as f:
    lines = f.read().splitlines() 
    file_name = lines[0]
    array_name = lines[1]
    colorbar_title = lines[2]

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# get the material library
materialLibrary1 = GetMaterialLibrary()

# Create a new 'Render View'
renderView1 = CreateView('RenderView')
renderView1.ViewSize = [1489, 836]
renderView1.AxesGrid = 'GridAxes3DActor'
renderView1.StereoType = 'Crystal Eyes'
renderView1.CameraPosition = [61.14866875195395, -57.07488151038765, 29.93519975423152]
renderView1.CameraFocalPoint = [-7.462680059833104, 5.221139552758791, -1.8772247549205776]
renderView1.CameraViewUp = [-0.24091466376930126, 0.21766205376628572, 0.9458241671321193]
renderView1.CameraFocalDisk = 1.0
renderView1.CameraParallelScale = 25.35943525990673
renderView1.BackEnd = 'OSPRay raycaster'
renderView1.OSPRayMaterialLibrary = materialLibrary1

SetActiveView(None)

# ----------------------------------------------------------------
# setup view layouts
# ----------------------------------------------------------------

# create new layout object 'Layout #1'
layout1 = CreateLayout(name='Layout #1')
layout1.AssignView(0, renderView1)

# ----------------------------------------------------------------
# restore active view
SetActiveView(renderView1)
# ----------------------------------------------------------------

# ----------------------------------------------------------------
# setup the data processing pipelines
# ----------------------------------------------------------------

# create a new 'PVD Reader'
output = PVDReader(FileName=file_name)
output.CellArrays = ['Label']
output.PointArrays = ['phi', 'Mg', 'F', 'Cl', 'OH']

# create a new 'Calculator'
calculate_pH = Calculator(Input=output)
calculate_pH.ResultArrayName = 'pH'
calculate_pH.Function = '14+log10(OH)'

# create a new 'Clip'
solution = Clip(Input=calculate_pH)
solution.ClipType = 'Plane'
solution.HyperTreeGridClipper = 'Plane'
solution.Scalars = ['CELLS', 'Label']
solution.Value = -55555.5

# create a new 'Clip'
block = Clip(Input=output)
block.ClipType = 'Scalar'
block.HyperTreeGridClipper = 'Plane'
block.Scalars = ['POINTS', 'phi']
block.Invert = 0

# ----------------------------------------------------------------
# setup the visualization in view 'renderView1'
# ----------------------------------------------------------------

# show data from solution
solutionDisplay = Show(solution, renderView1, 'UnstructuredGridRepresentation')

# get color transfer function/color map for 'Mg'
mgLUT = GetColorTransferFunction('Mg')
mgLUT.RGBPoints = [0.0, 0.278431372549, 0.278431372549, 0.858823529412, 1.9162000000000028e-05, 0.0, 0.0, 0.360784313725, 3.819000000000005e-05, 0.0, 1.0, 1.0, 5.748600000000008e-05, 0.0, 0.501960784314, 0.0, 7.65140000000001e-05, 1.0, 1.0, 0.0, 9.567600000000013e-05, 1.0, 0.380392156863, 0.0, 0.00011483800000000016, 0.419607843137, 0.0, 0.0, 0.0001340000000000002, 0.878431372549, 0.301960784314, 0.301960784314]
mgLUT.ColorSpace = 'RGB'
mgLUT.ScalarRangeInitialized = 1.0

# get opacity transfer function/opacity map for 'Mg'
mgPWF = GetOpacityTransferFunction('Mg')
mgPWF.Points = [0.0, 0.0, 0.5, 0.0, 0.0001340000000000002, 1.0, 0.5, 0.0]
mgPWF.ScalarRangeInitialized = 1

# trace defaults for the display properties.
solutionDisplay.Representation = 'Surface'
solutionDisplay.ColorArrayName = ['POINTS', array_name]
solutionDisplay.LookupTable = mgLUT
solutionDisplay.OSPRayScaleArray = 'Cl'
solutionDisplay.OSPRayScaleFunction = 'PiecewiseFunction'
solutionDisplay.SelectOrientationVectors = 'Cl'
solutionDisplay.ScaleFactor = 3.5
solutionDisplay.SelectScaleArray = 'Label'
solutionDisplay.GlyphType = 'Arrow'
solutionDisplay.GlyphTableIndexArray = 'Label'
solutionDisplay.GaussianRadius = 0.17500000000000002
solutionDisplay.SetScaleArray = ['POINTS', 'Cl']
solutionDisplay.ScaleTransferFunction = 'PiecewiseFunction'
solutionDisplay.OpacityArray = ['POINTS', 'Cl']
solutionDisplay.OpacityTransferFunction = 'PiecewiseFunction'
solutionDisplay.DataAxesGrid = 'GridAxesRepresentation'
solutionDisplay.PolarAxes = 'PolarAxesRepresentation'
solutionDisplay.ScalarOpacityFunction = mgPWF
solutionDisplay.ScalarOpacityUnitDistance = 0.21000543165553062
solutionDisplay.ExtractedBlockIndex = 2

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
solutionDisplay.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 0.005175000000000006, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
solutionDisplay.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 0.005175000000000006, 1.0, 0.5, 0.0]

# show data from block
blockDisplay = Show(block, renderView1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
blockDisplay.Representation = 'Surface'
blockDisplay.ColorArrayName = ['POINTS', '']
blockDisplay.OSPRayScaleArray = 'Cl'
blockDisplay.OSPRayScaleFunction = 'PiecewiseFunction'
blockDisplay.SelectOrientationVectors = 'Cl'
blockDisplay.ScaleFactor = 2.537127685546875
blockDisplay.SelectScaleArray = 'Label'
blockDisplay.GlyphType = 'Arrow'
blockDisplay.GlyphTableIndexArray = 'Label'
blockDisplay.GaussianRadius = 0.12685638427734375
blockDisplay.SetScaleArray = ['POINTS', 'Cl']
blockDisplay.ScaleTransferFunction = 'PiecewiseFunction'
blockDisplay.OpacityArray = ['POINTS', 'Cl']
blockDisplay.OpacityTransferFunction = 'PiecewiseFunction'
blockDisplay.DataAxesGrid = 'GridAxesRepresentation'
blockDisplay.PolarAxes = 'PolarAxesRepresentation'
blockDisplay.ScalarOpacityUnitDistance = 0.15830440730693082
blockDisplay.ExtractedBlockIndex = 2

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
blockDisplay.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 0.005174984190280608, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
blockDisplay.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 0.005174984190280608, 1.0, 0.5, 0.0]

# setup the color legend parameters for each legend in this view

# get color legend/bar for mgLUT in view renderView1
mgLUTColorBar = GetScalarBar(mgLUT, renderView1)
mgLUTColorBar.WindowLocation = 'AnyLocation'
mgLUTColorBar.Position = [0.9220953660174613, 0.014354066985645933]
mgLUTColorBar.Title = colorbar_title
mgLUTColorBar.ComponentTitle = ''
mgLUTColorBar.ScalarBarLength = 0.5800000000000002

# set color bar visibility
mgLUTColorBar.Visibility = 1

# show color legend
solutionDisplay.SetScalarBarVisibility(renderView1, True)

solutionDisplay.RescaleTransferFunctionToDataRange(False, True)


# ----------------------------------------------------------------
# setup color maps and opacity mapes used in the visualization
# note: the Get..() functions create a new object, if needed
# ----------------------------------------------------------------

# ----------------------------------------------------------------
# finally, restore active source
SetActiveSource(solution)
# ----------------------------------------------------------------
