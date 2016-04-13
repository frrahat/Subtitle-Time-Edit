#include "OpenFileDialog.h"
#include "SubtitleHandler.h"
#include<iostream>

using namespace std;

int main(){
	OpenFileDialog* openFileDialog1 = new OpenFileDialog();
	
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->Flags |= OFN_SHOWHELP;
	//openFileDialog1->InitialDir = _T("C:\\");
	openFileDialog1->Title = _T("Select the .srt File");
	
	cout<<"Opening file selection dialog"<<endl;
	if (openFileDialog1->ShowDialog())
	{
		MessageBox(0, openFileDialog1->FileName, _T("Confirm Selection"),
				   MB_OK | MB_ICONINFORMATION);

		cout<<"Selected : "<<openFileDialog1->FileName<<endl;
		//creatSubtitleFromTranscript(openFileDialog1->FileName);
		creatEditedSubtitleFile(openFileDialog1->FileName);
		MessageBox(0, "Check the same directory for the new subtitle file.",_T("Task Finished"),MB_OK);
	}
	cout<<"Exiting"<<endl;
	return 0;
}