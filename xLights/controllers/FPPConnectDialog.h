#ifndef FPPCONNECTDIALOG_H
#define FPPCONNECTDIALOG_H

#include <wx/progdlg.h>
#include <list>

//(*Headers(FPPConnectDialog)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checklst.h>
#include <wx/checkbox.h>
#include <wx/filepicker.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)

class OutputManager;

class FPPConnectionDetails
{
    std::string _password;
public:
    std::string _ip;
    std::string _user;
    std::string _description;
    FPPConnectionDetails(const std::string& ip, const std::string& user, const std::string& password, const std::string& description);
    bool IsDefaultPassword() const;
    std::string GetPassword() const;
    void SetPassword(const std::string& user, const std::string& password);
    bool operator==(const FPPConnectionDetails& c2) const { return  _ip == c2._ip; }
    bool operator==(const std::string& c2) const { return  _ip == c2; }
};

class FPPConnectDialog: public wxDialog
{
    std::list<FPPConnectionDetails> _connectionDetails;
    std::list<FPPConnectionDetails> _allConnectionDetails;
    OutputManager* _outputManager;
    bool _updating;
    void LoadSequencesFromFolder(wxString dir);
    void LoadSequences();
    bool CopyFile(std::string source, std::string target, bool backup, wxProgressDialog &progress, int start, int end);
    bool DoCopyFile(const std::string& source, const std::string& target, wxProgressDialog &progress, int start, int end);
    void ValidateWindow();
    bool IsValidIP(wxString ip);
    bool FTPUpload();
    bool USBUpload();
    void OnPopup(wxCommandEvent &event);
    void SaveConnectionDetails();
    void LoadConnectionDetails();

	public:

		FPPConnectDialog(wxWindow* parent, OutputManager* outputManager, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~FPPConnectDialog();

		//(*Declarations(FPPConnectDialog)
		wxNotebook* Notebook_FPP;
		wxCheckBox* CheckBox_UploadModels;
		wxButton* Button_Upload;
		wxTextCtrl* TextCtrl_Description;
		wxStaticText* StaticText2;
		wxPanel* Panel_USB;
		wxChoice* Choice_Drives;
		wxDirPickerCtrl* DirPickerCtrl_FPPMedia;
		wxStaticText* StaticText6;
		wxComboBox* ComboBox_IPAddress;
		wxButton* Button_Console;
		wxStaticText* StaticText1;
		wxCheckBox* CheckBox_UploadController;
		wxStaticText* StaticText3;
		wxButton* Button_Forget;
		wxStaticText* StaticText5;
		wxCheckListBox* CheckListBox_Sequences;
		wxPanel* Panel_FTP;
		wxStaticText* StaticText4;
		wxTextCtrl* TextCtrl_Password;
		wxButton* Button_UploadToAll;
		wxTextCtrl* TextCtr_Username;
		//*)

	protected:

		//(*Identifiers(FPPConnectDialog)
		static const long ID_STATICTEXT1;
		static const long ID_COMBOBOX_IPAddress;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL_Username;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL_Password;
		static const long ID_BUTTON_Console;
		static const long ID_PANEL_FTP;
		static const long ID_STATICTEXT4;
		static const long ID_CHOICE_Drives;
		static const long ID_STATICTEXT5;
		static const long ID_DIRPICKERCTRL1;
		static const long ID_PANEL_USB;
		static const long ID_NOTEBOOK_FPP;
		static const long ID_CHECKBOX_UploadController;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKLISTBOX_Sequences;
		static const long ID_BUTTON_Upload;
		static const long ID_BUTTON1;
		//*)

        static const long ID_MNU_SELECTALL;
        static const long ID_MNU_SELECTNONE;

	private:

		//(*Handlers(FPPConnectDialog)
		void OnButton_UploadClick(wxCommandEvent& event);
		void OnCheckListBox_SequencesToggled(wxCommandEvent& event);
		void OnCheckBox_UploadControllerClick(wxCommandEvent& event);
		void OnTextCtrl_IPAddressText(wxCommandEvent& event);
		void OnTextCtr_UsernameText(wxCommandEvent& event);
		void OnTextCtrl_PasswordText(wxCommandEvent& event);
		void OnNotebook_FPPPageChanged(wxNotebookEvent& event);
		void OnButton_ConsoleClick(wxCommandEvent& event);
		void OnFilePickerCtrl_MediaFolderFileChanged(wxFileDirPickerEvent& event);
		void OnCheckBox_UploadModelsClick(wxCommandEvent& event);
		void OnComboBox_IPAddressSelected(wxCommandEvent& event);
		void OnButton_UploadToAllClick(wxCommandEvent& event);
		void OnButton_ForgetClick(wxCommandEvent& event);
		void OnTextCtrl_DescriptionTextEnter(wxCommandEvent& event);
		void OnTextCtr_UsernameTextEnter(wxCommandEvent& event);
		void OnTextCtrl_PasswordTextEnter(wxCommandEvent& event);
		void OnComboBox_IPAddressTextEnter(wxCommandEvent& event);
		void OnTextCtrl_DescriptionText(wxCommandEvent& event);
		//*)

        void OnSequenceRClick(wxMouseEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif