#include<fstream>
#include<bits/stdc++.h>
#include<queue>
#include<string>
#include<iostream>
#include<iomanip>
using namespace std;
#include"bvh_parser.h"

// a naive bvh parser

using std::ifstream;
int Frame,col=0;
queue<string> Q;
vector<vector<string> > M;        

void CreTree(joint *parent){ 
	parent->joint_type=Q.front();      //TYPE & NAME
	Q.pop();
	parent->name=Q.front();
	Q.pop(); Q.pop();
	
	Q.pop();                          //OFFSET 
	stringstream x(Q.front());  //取队列当前的第一个字符串数据
	x>>parent->offset_x;     //同样使用了stringstream，将字符串数据写入到double变量
	Q.pop();                //每完成一次赋值，相应的字符串数据出队
	stringstream y(Q.front());
	y>>parent->offset_y;
	Q.pop();
	stringstream z(Q.front());
	z>>parent->offset_z;
	Q.pop();
	
	Q.pop();                          //CHANNELS 
    int Channels;
	stringstream ss(Q.front());
	ss>>Channels;       //首先用Channels变量记录之后CHANNEL数据的个数
    Q.pop();
    string c;
    for(int j=0;j<Channels;j++){
    	c=Q.front();        //利用中间变量c，按顺序完成对结点channels数组的赋值
    	parent->channels.push_back(c);
		Q.pop();
	}
	for(int i=0;i<Frame;i++){         //MOTION
		vector<double> subm;    //定义一个存储bouble类型数据的子数组
		for(int j=col;j<col+Channels;j++){
			double dm;
			dm=stod(M[i][j]);  //使用到了stod函数，可以将字符串转换成double类型
			subm.push_back(dm); 
		}
		parent->motion.push_back(subm);//二维嵌套数组的每一个元素就是刚刚建好的子数组
	}
	col+=Channels;  //见文字说明
	
	string sub;                       //Children
	sub=Q.front();
	while(sub[0]!='}'){   //如果队头已经是"}"，说明建树结束了，直接出队、return即可
	    joint *current=new joint;
		switch(sub[0]){    
            case('J'):{   //如果下一结点是JOINT类型，就用递归建立子树current
				CreTree(current);   //递归调用返回后，把子树存入parent的children数组
				parent->children.push_back(current);
				break;
			}
            case('E'):{  //如果下一结点是END类型的，要单独处理
            	current->joint_type=Q.front();
				Q.pop();
	            string n=parent->name;
	            n.append("_End");   //利用字符串函数append()给子结点命名
	            current->name=n;
			    Q.pop(); Q.pop(); Q.pop();          
				stringstream x(Q.front()); //之后offset的处理同上
		    	x>>current->offset_x;
			    Q.pop();
			    stringstream y(Q.front());
			    y>>current->offset_y;
			    Q.pop();
		    	stringstream z(Q.front());
			    z>>current->offset_z;
			    Q.pop();         
            	parent->children.push_back(current);
			    Q.pop();  //处理完毕后，同样存入parent结点的children数组内
				break;
			}
			default:;
        }
        sub=Q.front();  //继续取队头，进入下一轮循环，看parent是否有第二个子结点
    }
    Q.pop();     //处理完当前结点的所有子结点，出队，函数返回
    return ;
}

int main(int argc, char** argv) {
    joint root;
    META meta_data;
    ifstream file(argv[1]);
    //do something
	 
    string s,t;                       //HIERARCHY
    getline(file,s);
    while(getline(file,s)){        //按行读入HIERARCHY部分数据
        if(!s.empty()){
            s.erase(0,s.find_first_not_of(" "));
    	    s.erase(s.find_last_not_of(" ") + 1);
        }      //去掉每一行主体内容前、后的空格
        if(s[0]=='M')break;
        istringstream ss(s);  //把每一行按空格分割为各子字符串存储
        while(ss>>t)
            Q.push(t);        //各子字符串按顺序入队
    }

    getline(file,s);               //Frame
    istringstream f(s);    //处理第一行的Frames，能力有限，办法很简陋
    f>>t; f>>t;            //就是先取出子字符串，将其格式转换成int类型，最后赋值
    stringstream f_(t);
	f_>>Frame;
	meta_data.frame=Frame;     
    getline(file,s);
    istringstream ft(s);
    ft>>t; ft>>t; ft>>t;
	stringstream ft_(t);    //同理处理Frame Time
	ft_>>meta_data.frame_time; 
    
    while(getline(file,s)){        //MOTION
    	istringstream m(s);
    	vector<string> MM;
        while(m>>t)        //把每一行的所有字符串数据插入辅助数组MM
        	MM.push_back(t);
		M.push_back(MM);   //每一个子数组MM作为元素插入二维嵌套数组M
	}	           
 
    CreTree(&root);        //运行主体函数，建树
	{
		vector<vector<string> > tmp;   
		M.swap(tmp);
    }                     //释放vector内存
    
    jsonify(root, meta_data);
    file.close();
    return 0;
}
