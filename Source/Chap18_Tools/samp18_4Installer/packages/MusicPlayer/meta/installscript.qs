function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install MusicPlayer.exe!
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@\\MusicPlayer.exe", "@StartMenuDir@\\Music Player.lnk",
            "workingDirectory=@TargetDir@");
		
        component.addOperation("CreateShortcut", "@TargetDir@\\MusicPlayer.exe", "@DesktopDir@\\Music Player.lnk",
            "workingDirectory=@TargetDir@");
    }
}
