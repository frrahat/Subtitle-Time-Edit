#include<stdio.h>
#include<string.h>
#include<conio.h>
int a[4];

void timeDiv(int time);
int timeEdit(int t[]);
int correction(char *line);
void createFileName(char *str,char *c1,char *p);
 int main()
{
	printf("***This program DELAYs or QUICKENs the time of appearance of subtitles while watching a movie.\n");
	printf("For this you need a SUBTITLE FILE(.srt or .txt).\n");
	printf("You have to enter the FILE PATH of the subtitle file\n");
	printf("and a time you want to quicken or delay.\n\n\t***Let's go\n\n");
    FILE *Old,*New;
	int c,time,flag,a[4],l;
    char str[300],c1[300],cp[300],line[1000];
    
    printf("***Enter file address.\n");   
open_file:
	printf("Example: E:\\\\Movies\\\\Ice Age 4\\\\Ice Age 4.srt\n\t-->");
	fgets(str,300,stdin);
	l=strlen(str);
	str[l-1]='\0';
    if((Old=fopen(str,"r"))==NULL)
	{
		printf("Wrong file path or No such File or Directory\n\n");
		printf("***Type file path again:\n");
		goto open_file;
	}

	else
	{
		printf("\n\n***Enter time in MILISECONDS.\n");
		printf("POSITIVE time to DELAY the appearance of subtitles\n");
		printf("And NEGATIVE time to QUICKEN the appearance.\n\n");
		printf("\tTime(in miliseconds): ");
		scanf("%d",&time);
		timeDiv(time);
		createFileName(str,c1,"(old)");
		
		if((New=fopen(c1,"r"))!=NULL)
		{
			fclose(New);
			printf("\n>>>>>Warning:a Recovery file already exists.\nFile path: %s\n",c1);
			printf("This is may be the Original subtitle file\n");
			printf("\nMove or Rename this file otherwise you may lose the main subtitle file.\n");
			printf("\n\n***Enter 1 to Continue.\n***Or enter 0 to EXIT.: "); 
			scanf("%d",&c);
			if(c==0)return 0;
		}

		New=fopen(c1,"w");
		while (fgets(line,1000,Old) != NULL)
		{
			if(strlen(line)>12 && line[2]==':' && line[5]==line[2])
			{
				flag=correction(line);
				if (flag)
				{
					printf("\n\nError: Input time delay is too much.\n");
					printf("Time editing UNSUCCESSFUL.\n\n");
					printf("***Try again.\n\n");
					break;
				}
			}
			fputs(line,New);
		}
		fclose(Old);
		fclose(New);
		
		strcpy(cp,c1);
		strcat(cp,"456345123.txt");
		
		rename(c1,cp);
		rename(str,c1);
		rename(cp,str);
		if(flag==0)printf("\n\n\t\tDONE!\n\nThank You\n**************************************\n");
		c=getchar();
		c=getche();
	}
	return 0;
    
}

int timeEdit(int t[])
{
	int i;
	if(t[3]>999){t[3]=999;t[2]++;}
	else if(t[3]<0){t[3]+=1000;t[2]--;}
	for(i=2;i>0;i--)
	{
		if(t[i]>59){t[i]=59;t[i-1]++;}
		else if(t[i]<0){t[i]+=t[i]+60;t[i-1]--;}
	}
	if (t[0]<0)return 1;
	return 0;
}

int correction(char *line)
{
	int i,j,t1[4],t2[4],flag;
	//////////
	for(i=0,j=0;j<3;i+=3,j++)
	{
		t1[j]=(line[i]-'0')*10+(line[i+1]-'0')+a[j];
	}
	t1[3]=(line[9]-'0')*100+(line[10]-'0')*10+(line[11]-'0')+a[j];
	////
	for(i=15;i<25;i++)if(line[i]>='0' && line[i]<='9')break;
	////
	for(j=0;j<3;i+=3,j++)
	{
		t2[j]=(line[i]-'0')*10+(line[i+1]-'0')+a[j];
	}
	t2[3]=(line[i]-'0')*100+(line[i+1]-'0')*10+(line[i+2]-'0')+a[j];
	//////////
	flag=timeEdit(t1);
	if (flag)return 1;
	flag=timeEdit(t2);
	//////////	
	for(i=0,j=0;j<3;i+=3,j++)
	{
		line[i]=t1[j]/10+'0';
		line[17+i]=t2[j]/10+'0';
		
		line[i+1]=t1[j]%10+'0';
		line[18+i]=t2[j]%10+'0';
		
		if(j!=2){line[i+2]=':';line[19+i]=':';}
		else {line[i+2]=',';line[19+i]=',';}
	}
	line[9] =t1[3]/100+'0';line[26]=t2[3]/100+'0';
	line[10]=(t1[3]%100)/10+'0';line[27]=(t2[3]%100)/10+'0';
	line[11]=t1[3]%10+'0';line[28]=t2[3]%10+'0';

	line[12]=' ';line[16]=' ';
	line[13]=line[14]='-';
	line[15]='>';
	line[29]='\n';
	line[30]='\0';
	return 0;
}

void createFileName(char *str1,char *str2,char *p)
{
	int i,j,k,m,lstr,lp;
	lstr=strlen(str1);
	lp=strlen(p);

	strcpy(str2,str1);
	for(i=lstr-1,j=0;i>0;i--,j++)if(str2[i]=='.')break;
	m=i;
	for(k=0;k<lp;i++,k++)str2[i]=p[k];
	for(;j>-1;m++,i++,j--)str2[i]=str1[m];
	str2[i]='\0';
	printf("\n\n\tRecovery file is being created......\n\tFile path: %s\n",str2);
}
void timeDiv(int time)
{
	int h,m,s,ms;
	s=time/1000;
	ms=time%1000;
	m=s/60;
	s%=60;
	h=m/60;
	m%=60;
	a[0]=h,a[1]=m,a[2]=s,a[3]=ms;
}