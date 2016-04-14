#pragma once

#include<fstream>
#include<string>
#include<iostream>
#include<vector>

using namespace std;

class Time{

public:
	int hh;
	int mm;
	int ss;
	int ms;
	
	Time(){
		this->hh=0;
		this->mm=0;
		this->ss=0;
		this->ms=0;
	}

	Time(int hh,int mm,int ss,int ms){
		this->hh=hh;
		this->mm=mm;
		this->ss=ss;
		this->ms=ms;
	}

	Time(int timeInMiliSecond){
		int t=timeInMiliSecond;
		this->hh=timeInMiliSecond/3600000;
		timeInMiliSecond%=3600000;
		this->mm=timeInMiliSecond/60000;
		timeInMiliSecond%=60000;
		this->ss=timeInMiliSecond/1000;
		timeInMiliSecond%=1000;
		this->ms=timeInMiliSecond;
	}

	string toString(){
		string time="";
		time+=hh/10+'0';
		time+=hh%10+'0';
		
		time+=':';

		time+=mm/10+'0';
		time+=mm%10+'0';
		
		time+=':';

		time+=ss/10+'0';
		time+=ss%10+'0';
		
		time+=',';

		time+=ms/100+'0';
		time+=(ms%100)/10+'0';
		time+=ms%10+'0';

		return time;
	}
	
	double toSeconds(){
		return hh*3600+mm*60+ss+(ms*1.0)/1000;
	}
};

class SubtitleTime{
public:
	int startTime[4];//hh:mm:ss:miliSec
	int endTime[4];//hh:mm:ss:miliSec

	SubtitleTime(string timeString){//00:00:25,000 --> 00:00:29,800
		
		//getting startTime
		//hh index 0,1
		//mm index 3,4
		//ss index 6,7
		for(int i=0,j=0;j<3;i+=3,j++){//hh,mm,ss
			startTime[j]=(timeString[i]-'0')*10+(timeString[i+1]-'0');
		}
		//miliSec index 9,10,11
		startTime[3]=(timeString[9]-'0')*100+(timeString[10]-'0')*10+(timeString[11]-'0');
		
		int len=timeString.length();
		int st=15;//end time start index
		for(;st<len;st++){//ignoring whitespaces or others
			if(timeString[st]>='0' && timeString[st]<='9')
				break;
		}

		//getting end time
		//hh index st+0,st+1
		//mm index st+3,st+4
		//ss index st+6,st+7
		for(int i=0,j=0;j<3;i+=3,j++){
			endTime[j]=(timeString[st+i]-'0')*10+(timeString[st+i+1]-'0');
		}
		//miliSec index st+9,st+10,st+11
		endTime[3]=(timeString[st+9]-'0')*100+(timeString[st+10]-'0')*10+(timeString[st+11]-'0');
	}

	string toString(){
		char timeString[30];

		//putting startTime
		for(int i=0,j=0;j<3;i+=3,j++){//hh,mm,ss
			timeString[i]=startTime[j]/10+'0';
			timeString[i+1]=startTime[j]%10+'0';
		}
		timeString[2]=timeString[5]=':';
		timeString[8]=',';
		//miliSec
		timeString[9]=startTime[3]/100+'0';
		timeString[10]=(startTime[3]%100)/10+'0';
		timeString[11]=startTime[3]%10+'0';
		
		//putting "-->"

		timeString[12]=' ';
		timeString[13]='-';
		timeString[14]='-';
		timeString[15]='>';
		timeString[16]=' ';
		
		//putting endTime
		int st=17;//end time start index
		
		for(int i=0,j=0;j<3;i+=3,j++){
			timeString[st+i]=endTime[j]/10+'0';
			timeString[st+i+1]=endTime[j]%10+'0';
		}
		
		timeString[st+2]=timeString[st+5]=':';
		timeString[st+8]=',';
		//miliSec
		timeString[st+9]=endTime[3]/100+'0';
		timeString[st+10]=(endTime[3]%100)/10+'0';
		timeString[st+11]=endTime[3]%10+'0';

		timeString[st+12]='\0';

		return string(timeString);
	}
	
	void roundTime(int t[]){
		if(t[3]>999){t[3]=999;t[2]++;}
		else if(t[3]<0){t[3]+=1000;t[2]--;}
		for(int i=2;i>0;i--){
			if(t[i]>59){t[i]=59;t[i-1]++;}
			else if(t[i]<0){t[i]+=60;t[i-1]--;}
		}
	}

	void changeSubtitleTime(bool addition,Time deltaTime){
		int polarity=1;
		if(!addition){
			polarity=-1;
		}

		this->startTime[0]+=polarity*deltaTime.hh;
		this->startTime[1]+=polarity*deltaTime.mm;
		this->startTime[2]+=polarity*deltaTime.ss;
		this->startTime[3]+=polarity*deltaTime.ms;

		this->endTime[0]+=polarity*deltaTime.hh;
		this->endTime[1]+=polarity*deltaTime.mm;
		this->endTime[2]+=polarity*deltaTime.ss;
		this->endTime[3]+=polarity*deltaTime.ms;

		roundTime(startTime);
		roundTime(endTime);

	}

	bool isValidTime(){
		for(int i=0;i<4;i++){
			if(startTime[i]<0){
				return false;
			}
		}
		return true;
	}

	Time getStartTime(){
		return Time(startTime[0],startTime[1],startTime[2],startTime[3]);
	}
};


//=======================================================
bool is_a_time_string(string line){
	int len=line.length();
	if(len<27)//least length can be 27
		return false;

	for(int i=0;i<len;i++){
		if((line[i]>='0' && line[i]<='9')){
			//do nothing
		}
		else{
			switch(line[i]){
				case ':':
				case ',':
				case '\n':
				case ' ': 
				case '-':
				case '>': break;
				default : return false;
			}
		}
	}
	return true;
}


//=======================================
string getNewSubtitleFileName(string oldFileName){
	int len=oldFileName.length();

	int lastDotPostion;
	for(lastDotPostion=len-1;lastDotPostion>=0;lastDotPostion--){
		if(oldFileName[lastDotPostion]=='.')
			break;
	}

	string subtitleFileName=oldFileName.substr(0,lastDotPostion)+"_(new)_";
	subtitleFileName.append(".srt");
	return subtitleFileName;
}



//==============================================
/*
* returns the integer if valid
*/
bool isInteger(string text){
	int len=text.length();
	for(int i=0;i<len;i++){
		if(text[i]<'0' || text[i]>'9')
			return false;
	}
	return true;
}
//==============================================

int getParsedInteger(string input){
	int len=input.length();

	int result=0;
	for(int i=0;i<len;i++){
		result*=10;
		result+=(input[i]-'0');
	}

	return result;
}

//==============================================
bool creatEditedSubtitleFile(string oldSubFileName){
	string line;
	ifstream oldSubFile(oldSubFileName.c_str());

	string newSubtitleFileName=getNewSubtitleFileName(oldSubFileName);
	cout<<"New Subtitle file will be : "<<newSubtitleFileName<<endl;
	
	//taking time input
	cout<<"===================================="<<endl;
	cout<<"Instructions:"<<endl;
	cout<<"1)Enter time in mili seconds or, in this format (hh:mm:ss,mmm) "<<endl;
	cout<<"2)Put - (minus sign) before the value or string if you want to decrease the time."<<endl;
	cout<<endl<<"Samples : 4500, -4500, 00:03:00,000, -00:03:00,000"<<endl<<endl;
	cout<<"Enter input : ";

	string input;
	cin>>input;
	
	bool isTimeAddition=true;
	Time inputTime=Time();
	if(input[0]=='-'){
		isTimeAddition=false;
	}
	if(!isInteger(input.substr(0,1))){
		input=input.substr(1,input.length());
	}
	//input in miliseconds

	if(isInteger(input)){
		inputTime=Time(getParsedInteger(input));
	}else{
		SubtitleTime deltaSubTime(input+"-->00:00:00,000");
		inputTime=Time(deltaSubTime.getStartTime());
	}
	
	cout<<"Time will be "<<(isTimeAddition?"added":"subtracted")<<
		" = "<<inputTime.toString()<<endl<<endl;

	ofstream newSubtitleFile(newSubtitleFileName.c_str());
	
	int subTitleCount=0;
	bool nextIsText=false;
	bool isValidTimeFound=false;
	if (oldSubFile.is_open() && newSubtitleFile.is_open())
	{
		while (getline (oldSubFile,line))
		{
			if(isInteger(line)){
				nextIsText=false;
			}
			else if(is_a_time_string(line)){
				SubtitleTime sTime(line);
				
				//change time
				sTime.changeSubtitleTime(isTimeAddition,inputTime);

				if(!isValidTimeFound){
					if(sTime.isValidTime())
						isValidTimeFound=true;
					else
						continue;
				}
				
				subTitleCount++;

				newSubtitleFile<<endl<<subTitleCount<<endl;
				newSubtitleFile<<sTime.toString()<<endl;
				
				nextIsText=true;
			}
			else if(nextIsText){
				newSubtitleFile<<line<<endl;
			}
		}
		oldSubFile.close();
		newSubtitleFile.close();

		cout<<"Successful"<<endl;
	}

	
	else cout << "Unable to open file"; 

	return true;
}