This sample demonstrates how to view block in custom dialog using AcGiDrawable implementation, this project works for all versions of AutoCAD, esp from AutoCAD 2022 which supports DX12.
Early version of Block view sample that shipped with ObjectARX SDK used GDI to draw over Window.

This sample is refactored to work with new DirectX API.

### Block View Sample Readme

- Compile and load the AsdkBlockView.arx.
- Run command BView at the AutoCAD command line.
- Default operation is to display the current drawing using the current view settings of the current drawing.

Function Descriptions
---------------------

- File->Open    
  
  - Opens an existing drawing into the Block View dialog (by clearing the AcGsView).

- File->Output    
  
  - Allows you to output a JPG;BMP;Tiff;Png files as a snaphot of the current view shown in the BlockView dialog.
    
        This feature requires the ATIL SDK.
        To enable the feature, follow the instructions in CBlockViewDlg::OnFilePrint().

- File->AcGsDevice
  
  - Config View or edit the current AcGsDevice configuration settings.

- View->Zoom 
  
  - Allows Zoom Extents/Zoom Window functionality

- View->Settings->Show
  
  - Allows the toggling of various GSView settings such as Linetype/Material and Sectioning

- View->Settings->Renderer Type
  
  - Allows you to switch the Renderer type.

- ViewModes
  
  - This dropdown allows the user to change the View mode of the AcGsView.
    
        It has the same options as the SHADEMODE command.

- ReMap Colors    
  
  - Button to remap the color palette.
    
        This is particularily useful if you want to show an AcGsView as a Paper Space layout, with
        a white background. In this case you will need to remap white entities to appear in a different color.

- Standard Colors
  
  - Restores the Color Palette back to the original one.

- Add Entity
  
  - Adds an entity to the Model Space and adds to the AcGsView.

- Add Temp Entity
  
  - Adds an entity just to the AcGsView.

## Build Instruction

```bash
git clone https://github.com/MadhukarMoogala/BlockView.git
cd BlockView
set ARXSDK=<Path of ObjectARX SDK>
msbuild /t:build /p:Configuration=Debug2023;Platform=x64 BlockView.vcxproj
```

## Demo

https://user-images.githubusercontent.com/6602398/199988476-434f757d-25cc-4a37-a293-7d0e4eee4576.mp4

## License

This sample is licensed under the terms of the [MIT License](http://opensource.org/licenses/MIT). Please see the [LICENSE](https://github.com/MadhukarMoogala/BlockView/blob/main/LICENSE) file for full details.

## Written By

Madhukar Moogala, [Forge Partner Development](http://forge.autodesk.com/) @galakar
