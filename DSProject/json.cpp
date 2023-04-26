#include<fstream>
#include<bits/stdc++.h>
#include<string>
#include<iostream>
using namespace std;
#include"bvh_parser.h"

using std::ofstream;
ofstream outfile;
int frame;

void PriTree(joint *parent,string *blank){
	*blank+="    ";
	outfile<<*blank<<'{'<<endl;
	*blank+="    ";
	outfile<<*blank<<"\"type\""<<": \""<<parent->joint_type<<"\","<<endl;
	outfile<<*blank<<"\"name\""<<": \""<<parent->name<<"\","<<endl;
	outfile<<*blank<<"\"offset\""<<": ["<<parent->offset_x<<", ";
	outfile<<parent->offset_y<<", "<<parent->offset_z<<"],"<<endl;
	outfile<<*blank<<"\"channels\""<<": [";
	int c=parent->channels.size();
	int i,j;
	for(i=0;i<c-1;i++) 
	    outfile<<"\""<<parent->channels[i]<<"\", ";
	outfile<<"\""<<parent->channels[i]<<"\"],"<<endl;
	outfile<<*blank<<"\"motion\""<<": ["<<endl;   //先输出motion部分的提示信息
	*blank+="    ";
	for(i=0;i<frame-1;i++){           //外循环frame-1次
		outfile<<*blank<<"[";
		for(j=0;j<c-1;j++)             //内循环c-1次，输出当前结点一帧下每个通道的值
			outfile<<parent->motion[i][j]<<", ";   //除了最后一个通道，前面的输出后都要加上逗号
		outfile<<parent->motion[i][j]<<"],"<<endl; //最后一个通道输出后加上回括号、逗号并换行
	}
	outfile<<*blank<<"[";             //最后一帧单独输出
	for(j=0;j<c-1;j++)
		outfile<<parent->motion[i][j]<<", ";
	outfile<<parent->motion[i][j]<<"]"<<endl;  //最后一个通道输出后加上回括号，换行
	blank->erase(0,4);                //motion部分全都输出结束后，缩进要回退
	outfile<<*blank<<"],"<<endl;      //并输出整个motion的回括号，换行
	
	int siz=parent->children.size();
	outfile<<*blank<<"\"children\""<<": ["<<endl;
	for(int k=0;k<siz;k++){
		joint *child=parent->children[k];
		if(child->joint_type=="JOINT"){
			PriTree(child,blank);
			if(k!=siz-1) outfile<<","<<endl;
			else outfile<<endl;
		}
		else if(child->joint_type=="End"){
			*blank+="    ";
			outfile<<*blank<<'{'<<endl;
			*blank+="    ";
			outfile<<*blank<<"\"type\""<<": \""<<child->joint_type<<"\","<<endl;
			outfile<<*blank<<"\"name\""<<": \""<<child->name<<"\","<<endl;
			outfile<<*blank<<"\"offset\""<<": ["<<child->offset_x<<", ";
			outfile<<child->offset_y<<", "<<child->offset_z<<"],"<<endl;
			outfile<<*blank<<"\"channels\""<<": [],"<<endl;
			outfile<<*blank<<"\"motion\""<<": ["<<endl;
			*blank+="    ";
			for(i=0;i<frame-1;i++)
				outfile<<*blank<<"[],"<<endl;
			outfile<<*blank<<"[]"<<endl;
			blank->erase(0,4);
			outfile<<*blank<<"],"<<endl;
			outfile<<*blank<<"\"children\""<<": ["<<endl;
			outfile<<endl;
			outfile<<*blank<<"]"<<endl;
			blank->erase(0,4);
		    outfile<<*blank<<"}"<<endl;
			outfile<<endl;
		    blank->erase(0,4);
		}
	}
	outfile<<*blank<<"]"<<endl;
	blank->erase(0,4);
	outfile<<*blank<<"}"<<endl;
	blank->erase(0,4);
	return ;
}

void jsonify(joint root, META meta_data) {
    outfile.open("output.json");
    
    // output the root and meta_data
    string blank="    ";
    outfile<<'{'<<endl;
	outfile<<blank<<"\"frame\""<<": "<<meta_data.frame<<","<<endl;
	frame=meta_data.frame;
	outfile<<blank<<"\"frame_time\""<<": "<<meta_data.frame_time<<","<<endl;
	outfile<<blank<<"\"joint\""<<": "<<endl;
	PriTree(&root,&blank); 
	outfile << "}" <<endl;
    outfile.close();
    return ;
}





