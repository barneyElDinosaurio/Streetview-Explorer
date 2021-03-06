#include "MainFrame.h"
#include "Application.h"
#include <wx/stdpaths.h>
#include "Settings.h"
#if __WXMAC__
#include <ApplicationServices/ApplicationServices.h>
#endif

#if __WXMSW__

void SetStdOutToNewConsole() {
    int hConHandle;
    long lStdHandle;
    FILE *fp;

    // allocate a console for this app
    AllocConsole();

    // redirect unbuffered STDOUT to the console
    lStdHandle = (long) GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    *stdout = *fp;

    setvbuf(stdout, NULL, _IONBF, 0);
}
#endif

bool Application::OnInit() {
    //Set the working directory to a place where it is suitable to place data files
    wxString dataDir = wxStandardPaths::Get().GetUserDataDir();
    wxMkDir(dataDir.char_str(), 0755);
    wxSetWorkingDirectory(dataDir);
    //wxMessageBox(dataDir);
	
    //Load settings
    settings = Settings();
    
    //Make sure there is a cache directory in the working directory
    ::wxMkDir("cache", 0755);
    
    wxImage::AddHandler(new wxPNGHandler);

#if __WXMSW__
    //SetStdOutToNewConsole();
#endif

#if __WXMAC__
    /**
     * This code serves no purpose other than debugging.
     * A wxWidgets program running outside a bundle is like a ghost,
     * it can't receive input and it can't have focus.
     * Because I don't want to package my app in a bundle every time I test
     * something out I run this code which brings back normal behaviour even without the bundle
     */
    ProcessSerialNumber PSN;
    GetCurrentProcess(&PSN);
    TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
#endif

    //Create the mainframe which creates the rest
    new MainFrame();

    return true;
}

IMPLEMENT_APP(Application)
