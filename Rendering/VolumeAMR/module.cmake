vtk_module(vtkRenderingVolumeAMR
  KIT
    vtkParallel
  DEPENDS
    vtkImagingCore
    vtkRenderingVolume
    vtkRenderingVolumeOpenGL2
  PRIVATE_DEPENDS
    vtkCommonCore
    vtkCommonDataModel
    vtkCommonExecutionModel
    vtkCommonMath
    vtkCommonSystem
    vtkFiltersAMR
    vtkRenderingCore
  )
