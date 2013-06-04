#include<iostream>
//#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glut.h>
#include<stdio.h>
#include<string>

using namespace std;
//using std::ifstream;

typedef struct pt
{
	float x,y;
}point;

typedef struct eq
{
	float A,B,C;
}equation;

#define PI 3.14159265
#define Zero 0.001

unsigned char selected;
float *proj_len;
float *mirror_len;
float *block_len;
float *src_dis;
float *proj_cos_angle;
float *proj_sin_angle;
float *mirror_cos_angle;
float *mirror_sin_angle;
float *block_cos_angle;
float *block_sin_angle;
float mirror_dis=10.0;
float block_dis=10.0;
float win_width;
float win_height;
float theta;
float change_x;
float change_y;
float trans_speed=2.0;
float rotate_speed=5.0;
float update_speed=10.0;
float extra;
float margin_x;
float margin_y;
float mouse_x;
float mouse_y;
float highlight_factor;
float dis;

float a,b;
int highlight_flag;
int index_obj;
int obj_selected;             // 0 for mirror , 1 for block , 2 for proj
int selected_by_key=0;
int random_mode=0;
int gaze_mode=0;
int resize_mode=0;
int light_transport_mode=0;
int no_of_proj=2;
int no_of_blocks=3;
int no_of_mirrors=2;
float world_width;
float world_height;
int change_color=0;
int stipple;
int range_x,range_y;
int *no_of_pt;
int pointer=0;

char data[100];

point *proj_src_point;
point *mirror_src_point;
point *block_src_point;
point *** line_point;
point ** block_point;
point ** mirror_point;
point ** mirror_block_point;
point ** proj_point;
point *check_point;

equation *proj_eq;

FILE *in,*out;

void initRendering();
void drawScene();
void defineMirror();
void defineProj();
void defineBlock();
void randomMode();
void selectedObjByKeyBoard();
void selectedObjByMouse();
void windowSize();
void gazeCursorToProjector();
void update(int value);
void drawBox(float wid,float hig);
void drawLine(point p1,point p2);
void drawSpittle(point p1,point p2);
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void getMousePos(int x,int y);
void translate(point **object_point,int index,int x,int y,float *object_cos_angle,float *object_sin_angle,float *object_len);
void rotate(point **object_point,float *object_cos_angle,float *object_sin_angle,float *object_len,int index,int dir);
void drawMirrorBack(point p1,point p2,point p3,point p4);
void highlight(point **object_point,float *object_cos_angle,float *object_sin_angle,int index);
void resizeObj(point **object_point,int index,int x,int y,float *object_cos_angle,float *object_sin_angle,float *object_len);
void changeProjCount(int del_add);
void changeMirrorCount(int del_add);
void changeBlockCount(int del_add);
void input();
void output();
void scan();
void changePixelCount(int del_add);
float distancePoint(point p1,point p2);
float max(float a,float b);
float min(float a,float b);
float perpendicularDistance(point p1,equation e1);


point  findIntersection(point p1,point p2,point p3,point p4);
point newPoint(equation e1,equation e2,point p1,float w,float h);

equation eqLine(point p1,point p2);

int checkSide(point p1,point p2,equation e1);
int checkRange(point p1,point p2,float x,float y,float extra);

void scan(){
//copied
	while(1){
		if(fscanf(in," %[^\n]",data)==EOF)
			break;
		istringstream line;
		line.str(data);
		float temp; 
		line >> temp;
		if(temp) 
			break;
	}
}

void input(){
	int i;
	printf("hello%d\n",no_of_proj);
	istringstream line;
	scan();
	line.str(data);
	line >> world_width;
	line >> world_height;
	scan();
	line.str(data);
	line >> no_of_proj;
	printf("%d\n",no_of_proj);

	proj_point=(point **)malloc(sizeof(point*)*no_of_proj);
	for(i=0;i<no_of_proj;i++)
		proj_point[i]=(point *)malloc(sizeof(point)*2);
	proj_src_point=(point *)malloc(sizeof(point)*no_of_proj);
	proj_len=(float *)malloc(sizeof(float)*no_of_proj);
	proj_cos_angle=(float *)malloc(sizeof(float)*no_of_proj);
	proj_sin_angle=(float *)malloc(sizeof(float)*no_of_proj);
	proj_eq=(equation *)malloc(sizeof(equation)*no_of_proj);
	src_dis=(float *)malloc(sizeof(float)*no_of_proj);
	check_point=(point *)malloc(sizeof(point)*no_of_proj);
	no_of_pt=(int *)malloc(sizeof(int)*no_of_proj);


	for(i=0;i<no_of_proj;i++){
		scan();
		line.str(data);
		line >> proj_point[i][0].x;
		line >> proj_point[i][0].y;
		line >> proj_point[i][1].x;
		line >> proj_point[i][1].y;
		printf("%f %f %f %f\n",proj_point[i][0].x,proj_point[i][0].y,proj_point[i][1].x,proj_point[i][1].y);
		scan();
		line.str(data);
		line >> src_dis[i];
		scan();
		line.str(data);
		line >> no_of_pt[i];
	}
	scan();
	line.str(data);
	line >> no_of_blocks;

	block_src_point=(point *)malloc(sizeof(point)*no_of_blocks);
	block_len=(float *)malloc(sizeof(float)*no_of_blocks);
	block_cos_angle=(float *)malloc(sizeof(float)*no_of_blocks);
	block_sin_angle=(float *)malloc(sizeof(float)*no_of_blocks);
	block_point=(point **)malloc(sizeof(point*)*no_of_blocks);
	for(i=0;i<no_of_blocks;i++)
		block_point[i]=(point *)malloc(sizeof(point)*2);

	for(i=0;i< no_of_blocks;i++){
		scan();
		line.str(data);
		line >> block_point[i][0].x;
		line >> block_point[i][0].y;
		line >> block_point[i][1].x;
		line >> block_point[i][1].y;
	}
	scan();
	line.str(data);
	line >> no_of_mirrors;

	mirror_src_point=(point *)malloc(sizeof(point)*no_of_mirrors);
	mirror_len=(float *)malloc(sizeof(float)*no_of_mirrors);
	mirror_cos_angle=(float *)malloc(sizeof(float)*no_of_mirrors);
	mirror_sin_angle=(float *)malloc(sizeof(float)*no_of_mirrors);
	mirror_point=(point **)malloc(sizeof(point*)*no_of_mirrors);
	for(i=0;i<no_of_mirrors;i++)
		mirror_point[i]=(point *)malloc(sizeof(point)*2);
	mirror_block_point=(point **)malloc(sizeof(point*)*no_of_mirrors);
	for(i=0;i<no_of_mirrors;i++)
		mirror_block_point[i]=(point *)malloc(sizeof(point)*2);

	for(i=0;i<no_of_mirrors;i++){
		scan();
		line.str(data);
		line >> mirror_point[i][0].x;
		line >> mirror_point[i][0].y;
		line >> mirror_point[i][1].x;
		line >> mirror_point[i][1].y;
	}
	fclose(in);
}

void output(){
	int i;
	fprintf(out,"# All numbers are in floating point except for number of items\n");
	fprintf(out,"%f %f  # width (x) and height (y) of the world\n",world_width,world_height);
	fprintf(out,"%d # number of Projectors\n",no_of_proj);
	for(i= 0;i<no_of_proj;i++){
		fprintf(out,"%f %f %f %f    #Project %d line: x1 y1 x2 y2\n",proj_point[i][0].x,proj_point[i][0].y,proj_point[i][1].x,proj_point[i][1].y,i+1);
		fprintf(out,"%f  # Distance(d) to the point light source\n",src_dis[i]);
		fprintf(out,"%d   # Number of pixels in the Projector\n",no_of_pt[i]);
	}
	fprintf(out,"%d # number of Blocks\n",no_of_blocks);
	for(i= 0;i<no_of_blocks;i++)
		fprintf(out,"%f %f %f %f     # Block %d: x1 y1 x2 y2\n",block_point[i][0].x,block_point[i][0].y,block_point[i][1].x,block_point[i][1].y,i+1);
	fprintf(out,"%d # number of Mirror\n",no_of_mirrors);
	for(i= 0;i<no_of_mirrors;i++)
		fprintf(out,"%f %f %f %f    # Mirror %d: x1 y1 x2 y2 \n",mirror_point[i][0].x,mirror_point[i][0].y,mirror_point[i][1].x,mirror_point[i][1].y,i+1);
	fclose(out);
}

void changePixelCount(int del_add){
	int k,i,j;
	point ***temp_line_point;
	temp_line_point=line_point;

	line_point=(point ***)malloc(sizeof(point**)*no_of_proj);
	for(k=0;k<no_of_proj;k++){
		line_point[k]=(point **)malloc(sizeof(point*)*no_of_pt[k]);
		for(i=0;i<no_of_pt[k];i++)
			line_point[k][i]=(point *)malloc(sizeof(point)*2);
	}

	for(i=0;i<no_of_proj;i++){
		j=no_of_pt[i];
		if(i==index_obj){
		if(del_add==1)
			j++;
		if(del_add==0)
			j--;
		}
		for(k=0;k<j;k++)
			free(temp_line_point[i][k]);
	}
}

void changeMirrorCount(int del_add){
	int i=0,j=0,k=0,index=index_obj;
	float *temp_len,*temp_cos_angle,*temp_sin_angle;
	point *temp_src_point, **temp_point,**temp_block_point;

	temp_len=mirror_len;
	temp_cos_angle=mirror_cos_angle;
	temp_sin_angle=mirror_sin_angle;
	temp_src_point=mirror_src_point;
	temp_point=mirror_point;
	temp_block_point=mirror_block_point;

	mirror_src_point=(point *)malloc(sizeof(point)*no_of_mirrors);
	mirror_len=(float *)malloc(sizeof(float)*no_of_mirrors);
	mirror_cos_angle=(float *)malloc(sizeof(float)*no_of_mirrors);
	mirror_sin_angle=(float *)malloc(sizeof(float)*no_of_mirrors);	
	
	mirror_point=(point **)malloc(sizeof(point*)*no_of_mirrors);
	for(i=0;i<no_of_mirrors;i++)
		mirror_point[i]=(point *)malloc(sizeof(point)*2);
	
	mirror_block_point=(point **)malloc(sizeof(point*)*no_of_mirrors);
	for(i=0;i<no_of_mirrors;i++)
		mirror_block_point[i]=(point *)malloc(sizeof(point)*2);
	// delete
	if(del_add==1){
		j=0;
		for(i=0;i<no_of_mirrors;i++){
			if(index==i)
				j++;
			for(k=0;k<2;k++){
				mirror_point[i][k].x=temp_point[j][k].x;
				mirror_point[i][k].y=temp_point[j][k].y;
			}
			j++;
		}
		for(i=0;i<no_of_mirrors;i++){
			free(temp_point[i]);
			free(temp_block_point[i]);
		}
	}else{
		j=0;
		for(i=0;i<no_of_mirrors-1;i++){
			for(k=0;k<2;k++){
				mirror_point[i][k].x=temp_point[j][k].x;
				mirror_point[i][k].y=temp_point[j][k].y;
			}
			j++;
		}
		mirror_point[i][0].x=-world_width/10.0;
		mirror_point[i][0].y=0;
		mirror_point[i][1].x=world_width/10.0;
		mirror_point[i][1].y=0;

		for(i=0;i<no_of_mirrors-1;i++){
			free(temp_point[i]);
			free(temp_block_point[i]);
		}
	}

	defineMirror();
	free(temp_cos_angle);
	free(temp_sin_angle);
	free(temp_len);
	free(temp_src_point);
}

void changeBlockCount(int del_add){
	float *temp_len,*temp_cos_angle,*temp_sin_angle;
	point *temp_src_point, **temp_point;
	int i=0,j=0,k=0,index;
	index=index_obj;
	temp_len=block_len;
	temp_cos_angle=block_cos_angle;
	temp_sin_angle=block_sin_angle;
	temp_src_point=block_src_point;
	temp_point=block_point;

	block_point=(point **)malloc(sizeof(point*)*no_of_blocks);
	for(i=0;i<no_of_blocks;i++)
		block_point[i]=(point *)malloc(sizeof(point)*2);
	
	block_sin_angle=(float *)malloc(sizeof(float)*no_of_blocks);
	block_cos_angle=(float *)malloc(sizeof(float)*no_of_blocks);
	block_len=(float *)malloc(sizeof(float)*no_of_blocks);
	block_src_point=(point *)malloc(sizeof(point)*no_of_blocks);

	//delete
	if(del_add==1){
		for(i=0;i<no_of_blocks;i++){
			if(index==i)
				j++;
			for(k=0;k<2;k++){
				block_point[i][k].x=temp_point[j][k].x;
				block_point[i][k].y=temp_point[j][k].y;
			}
			j++;
		}
		for(i=0;i<=no_of_blocks;i++)
			free(temp_point[i]);
	}else{
		j=0;
		for(i=0;i<no_of_blocks-1;i++){
			for(k=0;k<2;k++){
				block_point[i][k].x=temp_point[j][k].x;
				block_point[i][k].y=temp_point[j][k].y;
			}
			j++;
		}
		block_point[i][0].x=-world_width/10.0;
		block_point[i][0].y=0;
		block_point[i][1].x=world_width/10.0;
		block_point[i][1].y=0;

		for(i=0;i<no_of_blocks-1;i++)
			free(temp_point[i]);
	}
	defineBlock();
	free(temp_cos_angle);
	free(temp_sin_angle);
	free(temp_len);
	free(temp_src_point);

}

void changeProjCount(int del_add){
	int i,j,k,index=index_obj;
	float *temp_len,*temp_cos_angle,*temp_sin_angle,*temp_src_dis;
	point *temp_src_point, **temp_point,***temp_line_point,*temp_check_point;
	int *temp_no_of_pt;
	equation *temp_eq;

	temp_len=proj_len;
	temp_cos_angle=proj_cos_angle;
	temp_sin_angle=proj_sin_angle;
	temp_src_dis=src_dis;
	temp_src_point=proj_src_point;
	temp_point=proj_point;
	temp_line_point=line_point;
	temp_check_point=check_point;
	temp_no_of_pt=no_of_pt;
	temp_eq=proj_eq;

	proj_point=(point **)malloc(sizeof(point*)*no_of_proj);
	for(i=0;i<no_of_proj;i++)
		proj_point[i]=(point *)malloc(sizeof(point)*2);

	no_of_pt=(int *)malloc(sizeof(int)*no_of_proj);
	proj_eq=(equation *)malloc(sizeof(equation)*no_of_proj);
	check_point=(point *)malloc(sizeof(point)*no_of_proj);
	proj_sin_angle=(float *)malloc(sizeof(float)*no_of_proj);
	proj_cos_angle=(float *)malloc(sizeof(float)*no_of_proj);
	proj_src_point=(point *)malloc(sizeof(point)*no_of_proj);
	proj_len=(float *)malloc(sizeof(float)*no_of_proj);
	src_dis=(float *)malloc(sizeof(float)*no_of_proj);

	// delete
	if(del_add==1){
		j=0;
		for(i=0;i<no_of_proj;i++){
			if(index==i)
				j++;
			src_dis[i]=temp_src_dis[j];
			no_of_pt[i]=temp_no_of_pt[j];
			for(k=0;k<2;k++){
				proj_point[i][k].x=temp_point[j][k].x;
				proj_point[i][k].y=temp_point[j][k].y;
			}
			j++;
		}
		for(i=0;i<=no_of_proj;i++){
			free(temp_point[i]);
			for(k=0;k<temp_no_of_pt[i];k++)
				free(temp_line_point[i][k]);
		}
	}else{
		j=0;
		for(i=0;i<no_of_proj-1;i++){
			src_dis[i]=temp_src_dis[j];
			no_of_pt[i]=temp_no_of_pt[j];
			for(k=0;k<2;k++){
				proj_point[i][k].x=temp_point[j][k].x;
				proj_point[i][k].y=temp_point[j][k].y;
			}
			j++;
		}
		proj_point[i][0].x=-world_width/10.0;
		proj_point[i][0].y=0;
		proj_point[i][1].x=world_width/10.0;
		proj_point[i][1].y=0;
		src_dis[i]=world_width/20.0;
		no_of_pt[i]=6;

		for(i=0;i<no_of_proj-1;i++){
			free(temp_point[i]);
			for(k=0;k<temp_no_of_pt[i];k++)
				free(temp_line_point[i][k]);
		}
	}

	line_point=(point ***)malloc(sizeof(point**)*no_of_proj);
	for(k=0;k<no_of_proj;k++){
		line_point[k]=(point **)malloc(sizeof(point*)*no_of_pt[k]);
		for(i=0;i<no_of_pt[k];i++)
			line_point[k][i]=(point *)malloc(sizeof(point)*2);
	}

	free(temp_cos_angle);
	free(temp_sin_angle);
	free(temp_len);
	free(temp_src_point);
	free(temp_src_dis);
	free(temp_eq);
	free(temp_no_of_pt);
	free(temp_check_point);
	defineProj();
	return;
}


int main(int argc, char **argv) {

	// Initialize GLUT
	int i,k;
	in=fopen(argv[1],"r");
	out=fopen(argv[2],"w");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	win_width = glutGet(GLUT_SCREEN_WIDTH);
	win_height = glutGet(GLUT_SCREEN_HEIGHT);
	printf("%d %d\n",(int)win_width,(int)win_height);

	input();

	line_point=(point ***)malloc(sizeof(point**)*no_of_proj);
	for(k=0;k<no_of_proj;k++){
		line_point[k]=(point **)malloc(sizeof(point*)*no_of_pt[k]);
		for(i=0;i<no_of_pt[k];i++)
			line_point[k][i]=(point *)malloc(sizeof(point)*2);
	}

	glutInitWindowSize(win_width,win_height);
	glutInitWindowPosition(0,0); 
	highlight_factor=world_width/100.0;
	
	windowSize();
	defineProj();
	defineMirror();
	defineBlock();


	glutCreateWindow("----World Of Mirrors----");  // Setup the window
	initRendering();
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	glutPassiveMotionFunc(getMousePos);
	glutReshapeFunc(handleResize);
	glutTimerFunc(update_speed, update, 0);

	glutMainLoop();   
	return 0;
}

float distancePoint(point p1,point p2){
	return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

void windowSize()
{
	float a,b,c;
	if(world_width>win_width&&world_height>win_height){
		a=world_width/win_width;
		b=world_height/win_height;
		c=max(a,b);
		range_x=win_width*c;
		range_y=win_height*c;
	}else if(world_width>win_width&&world_height<win_height){
		a=world_width/win_width;
		c=a;
		range_x=win_width*c;
		range_y=win_height*c;
	}else if(world_width<win_width&&world_height>win_height){
		b=world_height/win_height;
		c=b;
		range_x=win_width*c;
		range_y=win_height*c;

	}else if(world_width<win_width&&world_height<win_height){
		a=win_width/world_width;
		b=win_height/world_height;
		c=min(a,b);
		range_x=win_width/c;
		range_y=win_height/c;

	}
}

void drawScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLineWidth(2.0*highlight_factor);
	glLoadIdentity();
	glPushMatrix();

	// Draw Box
	glColor3f(1.0f, 1.0f, 1.0f);
	drawBox(world_width,world_height);
	int i,j,k;

	glLineWidth(3.0);
	
	// Highlight Selected Object
	glColor3f(0.0f,1.0f,1.0f);
	if (highlight_flag==1){
		if (obj_selected==0){
			highlight(mirror_point,mirror_cos_angle,mirror_sin_angle,index_obj);
			highlight(mirror_block_point,mirror_cos_angle,mirror_sin_angle,index_obj);
		}else if (obj_selected==1){
			highlight(block_point,block_cos_angle,block_sin_angle,index_obj);
		}else if (obj_selected==2){
			highlight(proj_point,proj_cos_angle,proj_sin_angle,index_obj);
		}
	}	
	if(gaze_mode==1)
		gazeCursorToProjector();

	// Draw Blocks
	glColor3f(0.8f,0.4f,0.2f);
	for(i=0;i<no_of_blocks;i++)
		drawLine(block_point[i][0],block_point[i][1]);

	//Draw Mirrors BackSide
	glColor3f(0.8f,0.4f,0.2f);
	for(i=0;i<no_of_mirrors;i++)
		drawMirrorBack(mirror_block_point[i][0],mirror_block_point[i][1],mirror_point[i][1],mirror_point[i][0]);

	// Draw Mirrors
	glColor3f(0.7f,0.7f,0.7f);
	for(i=0;i<no_of_mirrors;i++)
		drawLine(mirror_point[i][0],mirror_point[i][1]);

	//Draw Lines
	for(k=0;k<no_of_proj;k++){
		check_point[k].x=proj_src_point[k].x;
		check_point[k].y=proj_src_point[k].y;
		float color_factor=2.0/(no_of_pt[k]-1);

		for(i=0;i<no_of_pt[k];i++){
			line_point[k][i][0].x=proj_src_point[k].x;
			line_point[k][i][0].y=proj_src_point[k].y;
			line_point[k][i][1].x=proj_point[k][0].x+((i+1)*proj_len[k]*proj_cos_angle[k])/((no_of_pt[k]+1)*1.0);
			line_point[k][i][1].y=proj_point[k][0].y+((i+1)*proj_len[k]*proj_sin_angle[k])/((no_of_pt[k]+1)*1.0);
		}
		equation temp;
		point tmp;
		proj_eq[k]=eqLine(proj_point[k][0],proj_point[k][1]);
		int mirrorflag=0,mirrorindex=0;
		
		for(i=0;i<no_of_pt[k];i++){
			temp=eqLine(line_point[k][i][0],line_point[k][i][1]);
			tmp=newPoint(temp,proj_eq[k],check_point[k],world_width,world_height);
			line_point[k][i][1].x=tmp.x;
			line_point[k][i][1].y=tmp.y;
			int flag=0,index=0;
			flag=0;
			
			for(j=0;j<no_of_blocks;j++){
				tmp=findIntersection(line_point[k][i][0],line_point[k][i][1],block_point[j][0],block_point[j][1]);
				if(tmp.x!=world_width && tmp.y!=world_height){
					line_point[k][i][1].x=tmp.x;
					line_point[k][i][1].y=tmp.y;
					flag=1;
				}
			}
			
			for(j=0;j<no_of_mirrors;j++){
				tmp=findIntersection(line_point[k][i][0],line_point[k][i][1],mirror_point[j][0],mirror_point[j][1]);
				equation mirror_temp;
				mirror_temp=eqLine(mirror_point[j][0],mirror_point[j][1]);
				if(tmp.x!=world_width && tmp.y!=world_height){
					if (mirrorflag==1&&mirrorindex==j)
						continue;

					if(((tmp.x==line_point[k][i][1].x)&&(tmp.y==line_point[k][i][1].y))||((tmp.x==line_point[k][i][0].x)&&(tmp.y==line_point[k][i][0].y)))
						continue;
					line_point[k][i][1].x=tmp.x;
					line_point[k][i][1].y=tmp.y;
					int reflect_or_block=0;
					reflect_or_block=checkSide(check_point[k],mirror_src_point[j],mirror_temp);
					if(reflect_or_block==1){
						flag=2;
						index=j;
					}
				}
			}
			if(i<no_of_pt[k]/2)
				glColor3f(1.0f-i*color_factor,i*i*color_factor*color_factor,0.0f);
			else
				glColor3f(0.0f,1.0f-(i-(no_of_pt[k]/2))*color_factor,(i-(no_of_pt[k])/2)*color_factor);
			if(light_transport_mode==0)
				drawLine(line_point[k][i][0],line_point[k][i][1]);
			else
				drawSpittle(line_point[k][i][0],line_point[k][i][1]);
			if (flag==2){
				tmp.x=line_point[k][i][1].x;
				tmp.y=line_point[k][i][1].y;
				line_point[k][i][1].y=line_point[k][i][0].y;
				line_point[k][i][1].x=line_point[k][i][0].x;
				line_point[k][i][0].x=tmp.x;
				line_point[k][i][0].y=tmp.y;
				equation mirror;
				mirrorflag=1;
				mirrorindex=index;
				mirror=eqLine(mirror_point[index][0],mirror_point[index][1]);
				proj_eq[k].A=mirror.A;
				proj_eq[k].B=mirror.B;
				proj_eq[k].C=mirror.C;
				tmp.x=line_point[k][i][1].x+((-2*(mirror.A*line_point[k][i][1].x+mirror.B*line_point[k][i][1].y+mirror.C)*mirror.A)/(mirror.A*mirror.A+mirror.B*mirror.B));
				tmp.y=line_point[k][i][1].y+((-2*(mirror.A*line_point[k][i][1].x+mirror.B*line_point[k][i][1].y+mirror.C)*mirror.B)/(mirror.A*mirror.A+mirror.B*mirror.B));
				line_point[k][i][1].x=tmp.x;
				line_point[k][i][1].y=tmp.y;
				check_point[k].x=tmp.x;
				check_point[k].y=tmp.y;

				i=i-1;
			}else{
				check_point[k].x=proj_src_point[k].x;
				check_point[k].y=proj_src_point[k].y;
				proj_eq[k]=eqLine(proj_point[k][0],proj_point[k][1]);
				mirrorflag=0;
			}
		}
	}

	// Draw Proj
	glColor3f(0.4f,0.5f,0.6f);
	for(i=0;i<no_of_proj;i++)
		drawLine(proj_point[i][0],proj_point[i][1]);

	glPopMatrix();
	glutSwapBuffers();
}

void drawBox(float wid ,float hig) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-wid / 2, -hig / 2);
	glVertex2f(wid / 2, -hig / 2);
	glVertex2f(wid / 2, hig / 2);
	glVertex2f(-wid / 2, hig / 2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawLine(point p1,point p2 ) {

	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, 0.0);
	glVertex3f(p2.x, p2.y, 0.0);
	glEnd();
}

void drawSpittle(point p1,point p2){
	glEnable(GL_LINE_STIPPLE);
	
	glLineStipple(3, (GLushort)stipple);
	glBegin(GL_LINE_STRIP);
	glVertex3f(p1.x, p1.y, 0.0);
	glVertex3f(p2.x, p2.y, 0.0);
	glEnd();
	
	glLineStipple(3, (GLushort)(stipple^0xffff));
	glColor3ub(1, 1, 1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(p1.x, p1.y, 0.0);
	glVertex3f(p2.x, p2.y, 0.0);
	glEnd();
	
	glDisable(GL_LINE_STIPPLE);

}

void drawMirrorBack(point p1,point p2,point p3,point p4){
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x,p2.y);
	glVertex2f(p3.x, p3.y);
	glVertex2f(p4.x,p4.y);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void highlight(point **object_point,float *object_cos_angle,float *object_sin_angle,int index){
	point p1,p2,p3,p4;
	float fct=highlight_factor;

	p1.x=object_point[index][0].x-fct*object_sin_angle[index];
	p2.x=object_point[index][1].x-fct*object_sin_angle[index];
	p3.x=object_point[index][1].x+fct*object_sin_angle[index];
	p4.x=object_point[index][0].x+fct*object_sin_angle[index];
	p1.y=object_point[index][0].y+fct*object_cos_angle[index];
	p2.y=object_point[index][1].y+fct*object_cos_angle[index];
	p3.y=object_point[index][1].y-fct*object_cos_angle[index];
	p4.y=object_point[index][0].y-fct*object_cos_angle[index];

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x,p2.y);
	glVertex2f(p3.x, p3.y);
	glVertex2f(p4.x,p4.y);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-range_x/2, range_x/2, -range_y/2, range_y/2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Initializing some openGL  rendering options
void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	//	glEnable( GL_LINE_SMOOTH );
	//	glEnable( GL_POLYGON_SMOOTH );
	//	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	//glMatrixMode(GL_PROJECTION);
	//	gluOrtho2D(-range_x/2, range_x/2, -range_y/2, range_y/2);
	//	glMatrixMode(GL_MODELVIEW);
	//	glViewport(0, 0, win_width, win_height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Setting a background color
}

point  findIntersection(point p1,point p2,point p3,point p4){
	point ret;
	ret.x=world_width;
	ret.y=world_height;

	// copied from " http://www.codersource.net/MFC/OpenGL/DetectinglinerectangleintersectioninOpenGL.aspx "

	// Given two lines, the first line is p1-p2  the second line is p3-p4

	float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
	float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	
	// If d is zero, there is no intersection
	if (d == 0) {
		return ret;
	}
	// Get the x and y
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

	// Check if the x and y coordinates are within both lines
	if ( x+Zero < min(x1, x2) || x-Zero > max(x1, x2) || x+Zero < min(x3, x4) || x-Zero > max(x3, x4) ) {
		return ret;
	}
	if ( y+Zero < min(y1, y2) || y-Zero > max(y1, y2) || y+Zero < min(y3, y4) || y-Zero > max(y3, y4) ) {
		return ret;
	}

	// Return the point of intersection
	ret.x = x;
	ret.y = y;
	return ret;
}

float max(float a,float b){
		return a>b?a:b;
}
float min(float a,float b){
		return a<b?a:b;
}

equation eqLine(point p1,point p2){
	equation temp;
	temp.A=p1.y-p2.y;
	temp.B=p2.x-p1.x;
	temp.C=p2.y*p1.x-p2.x*p1.y;
	return temp;
}

point newPoint(equation e1,equation e2,point p1,float w,float h){
	float temp;
	point pt;
	if (e1.A!=0){
		temp=-(e1.B*(h/2)+e1.C)/e1.A;
		if(temp<=w/2&& temp>=-w/2){
			pt.x=temp;
			pt.y=h/2;
			if(checkSide(p1,pt,e2)==0)
				return pt;
		}
		temp=-(e1.B*(-h/2)+e1.C)/e1.A;
		if(temp<=w/2&& temp>=-w/2){
			pt.x=temp;
			pt.y=-h/2;
			if(checkSide(p1,pt,e2)==0)
				return pt;
		}
	}
	if(e1.B!=0){
		temp=-(e1.A*(w/2)+e1.C)/e1.B;
		if((int)temp<=h/2&& (int)temp>=-h/2){
			pt.x=w/2;
			pt.y=temp;
			if(checkSide(p1,pt,e2)==0)
				return pt;
		}
		temp=-(e1.A*(-w/2)+e1.C)/e1.B;
		if(temp<=h/2&& temp>=-h/2){
			pt.x=-w/2;
			pt.y=temp;
			if(checkSide(p1,pt,e2)==0)
				return pt;
		}
	}
	pt.x=0.0;      // need to check for this later
	pt.y=0.0;
	return pt;
}

int checkSide(point p1,point p2,equation e1){
	if(((e1.A*p1.x+e1.B*p1.y+e1.C)>0&&(e1.A*p2.x+e1.B*p2.y+e1.C)>0)||((e1.A*p1.x+e1.B*p1.y+e1.C)<0&&(e1.A*p2.x+e1.B*p2.y+e1.C)<0)){
		return 1;
	}else if(((e1.A*p1.x+e1.B*p1.y+e1.C)<0&&(e1.A*p2.x+e1.B*p2.y+e1.C)>0)||((e1.A*p1.x+e1.B*p1.y+e1.C)>0&&(e1.A*p2.x+e1.B*p2.y+e1.C)<0)){
		return 0;
	}else {
		return 2;
	}
}

void defineMirror(){
	int i;
	float fct=highlight_factor;
	for(i=0;i<no_of_mirrors;i++){
		mirror_len[i]=distancePoint(mirror_point[i][0],mirror_point[i][1]);
		mirror_cos_angle[i]=(mirror_point[i][1].x-mirror_point[i][0].x)/mirror_len[i];
		mirror_sin_angle[i]=(mirror_point[i][1].y-mirror_point[i][0].y)/mirror_len[i];
		mirror_src_point[i].x=((mirror_point[i][0].x+mirror_point[i][1].x)/2.0 + (mirror_dis*mirror_sin_angle[i]));
		mirror_src_point[i].y=((mirror_point[i][0].y+mirror_point[i][1].y)/2.0 - (mirror_dis*mirror_cos_angle[i]));
	}
	for(i=0;i<no_of_mirrors;i++){
		mirror_block_point[i][0].x=mirror_point[i][0].x-fct*mirror_sin_angle[i];
		mirror_block_point[i][1].x=mirror_point[i][1].x-fct*mirror_sin_angle[i];
		mirror_block_point[i][0].y=mirror_point[i][0].y+fct*mirror_cos_angle[i];
		mirror_block_point[i][1].y=mirror_point[i][1].y+fct*mirror_cos_angle[i];
	}
}

void defineProj(){
	int i;
	for(i=0;i<no_of_proj;i++){
		proj_len[i]=distancePoint(proj_point[i][0],proj_point[i][1]);
		proj_cos_angle[i]=(proj_point[i][1].x-proj_point[i][0].x)/proj_len[i];
		proj_sin_angle[i]=(proj_point[i][1].y-proj_point[i][0].y)/proj_len[i];
		proj_src_point[i].x=((proj_point[i][0].x+proj_point[i][1].x)/2.0 + (src_dis[i]*proj_sin_angle[i]));
		proj_src_point[i].y=((proj_point[i][0].y+proj_point[i][1].y)/2.0 - (src_dis[i]*proj_cos_angle[i]));
	}
}

void defineBlock(){
	int i;
	for(i=0;i<no_of_blocks;i++){
		block_len[i]=distancePoint(block_point[i][0],block_point[i][1]);
		block_cos_angle[i]=(block_point[i][1].x-block_point[i][0].x)/block_len[i];
		block_sin_angle[i]=(block_point[i][1].y-block_point[i][0].y)/block_len[i];
		block_src_point[i].x=((block_point[i][0].x+block_point[i][1].x)/2.0 + (block_dis*block_sin_angle[i]));
		block_src_point[i].y=((block_point[i][0].y+block_point[i][1].y)/2.0 - (block_dis*block_cos_angle[i]));
	}
}

void handleKeypress1(unsigned char key, int x, int y) {
	selected=key;
	if (key == 27||key==113) { 
		output();
		exit(0);     // escape  key or q is pressed
	}
	if(key==122) {		// z is pressed
		if(random_mode==0){
			update_speed=100.0;
			margin_x=world_width/20.0;
			margin_y=world_height/20.0;
			random_mode=1;
		}else if(random_mode==1){
			update_speed=10.0;
			margin_x=world_width/30.0;
			margin_y=world_height/30.0;
			random_mode=0;
		}
	}
	if(key==98) {	  	// b is pressed
		no_of_blocks++;
		changeBlockCount(0);
		highlight_flag=1;
		obj_selected=1;
		index_obj=no_of_blocks-1;
	}
	if(key==109) {  	 // m is pressed
		no_of_mirrors++;
		changeMirrorCount(0);
		highlight_flag=1;
		obj_selected=0;
		index_obj=no_of_mirrors-1;
	}
	if(key==112) {	  	// p is pressed
		no_of_proj++;
		changeProjCount(0);
		highlight_flag=1;
		obj_selected=2;
		index_obj=no_of_proj-1;
	}
	if(key==97) { 	 	// a is pressed
		if(resize_mode==0){
			resize_mode=1;
		}else if(resize_mode==1){
			resize_mode=0;
		}
	}
	if(key==116) {	      // t is pressed
		if(light_transport_mode==0){
			light_transport_mode=1;
		}else if(light_transport_mode==1){
			light_transport_mode=0;
		}
	}
	if(key==103) {	     // g is pressed
		if(gaze_mode==0){
			highlight_flag=1;
			index_obj=0;
			obj_selected=2;
			gaze_mode=1;
		}else if(gaze_mode==1){
			gaze_mode=0;
			highlight_flag=0;
		}
	}
	if(key==99) { 	    // c is pressed
		highlight_flag=1;
		selectedObjByKeyBoard();
	}
	if(key==102) {      // f is pressed
		trans_speed+=1.0;
		rotate_speed+=1.0;
	}
	if (key==115) {	 	// s is pressed
		trans_speed-=1.0;
		rotate_speed-=1.0;
	}
	if(highlight_flag==1) {
		if (key==108) {   // l is pressed
			if (obj_selected==0)
				rotate(mirror_point,mirror_cos_angle,mirror_sin_angle,mirror_len,index_obj,1);
			else if (obj_selected==1)
				rotate(block_point,block_cos_angle,block_sin_angle,block_len,index_obj,1);
			else if (obj_selected==2)
				rotate(proj_point,proj_cos_angle,proj_sin_angle,proj_len,index_obj,1);
		}
		if (key==114) {    // r is pressed
			if (obj_selected==0)
				rotate(mirror_point,mirror_cos_angle,mirror_sin_angle,mirror_len,index_obj,-1);
			else if (obj_selected==1)
				rotate(block_point,block_cos_angle,block_sin_angle,block_len,index_obj,-1);
			else if (obj_selected==2)
				rotate(proj_point,proj_cos_angle,proj_sin_angle,proj_len,index_obj,-1);
		}
		if(key==127) {    //del is pressed
			if (obj_selected==0){
				if(no_of_mirrors>1){
					no_of_mirrors--;
					changeMirrorCount(1);
					index_obj=0;
				}
			}else if (obj_selected==1){
				if(no_of_blocks>1){
					no_of_blocks--;
					changeBlockCount(1);
					index_obj=0;
				}
			}else if (obj_selected==2){
				if(no_of_proj>1){
					no_of_proj--;
					changeProjCount(1);
					index_obj=0;
				}
			}
		}
		if (obj_selected==2) {
			if(key==117) {  // u is pressed
				if(no_of_pt[index_obj]>2){
					no_of_pt[index_obj]--;
					changePixelCount(1);
				}
			}
			if(key==105){
				no_of_pt[index_obj]++;
				changePixelCount(0);
			}
		}
		if(key==106){
			if(update_speed>10)
			update_speed-=10;
		}
		if(key==107)
			update_speed+=10;
	}
}

void getMousePos(int x, int y){
	a=win_width/range_x;
	b=win_height/range_y;
	x=(x-win_width/2)/a*1.0;
	y=(win_height/2-y)/b*1.0;
	mouse_x = x;
	mouse_y = y;
}

void handleKeypress2(int key, int x, int y) {
	a=win_width/range_x;
	b=win_height/range_y;
	x=(x-win_width/2)/a*1.0;
	y=(win_height/2-y)/b*1.0;
	mouse_x = x;
	mouse_y = y;

	if (key == GLUT_KEY_LEFT){
		change_x= -1;
		change_y=0;
	}else if (key == GLUT_KEY_RIGHT){
		change_x= 1;
		change_y=0;
	}else if (key == GLUT_KEY_UP){
		change_x=0;
		change_y = 1;
	}else if (key == GLUT_KEY_DOWN){
		change_x=0;
		change_y = -1;
	}else{
		change_x=0;
		change_y=0;
	}
	
	if(highlight_flag==1){
		if(resize_mode==1){
			if (obj_selected==0)
				resizeObj(mirror_point,index_obj,change_x,change_y,mirror_cos_angle,mirror_sin_angle,mirror_len);
			else if (obj_selected==1)
				resizeObj(block_point,index_obj,change_x,change_y,block_cos_angle,block_sin_angle,block_len);
			else if (obj_selected==2)
				resizeObj(proj_point,index_obj,change_x,change_y,proj_cos_angle,proj_sin_angle,proj_len);
		}else{
			if (obj_selected==0)
				translate(mirror_point,index_obj,change_x,change_y,mirror_cos_angle,mirror_sin_angle,mirror_len);
			else if (obj_selected==1)
				translate(block_point,index_obj,change_x,change_y,block_cos_angle,block_sin_angle,block_len);
			else if (obj_selected==2)
				translate(proj_point,index_obj,change_x,change_y,proj_cos_angle,proj_sin_angle,proj_len);
		}
	}
}

void handleMouseclick(int button, int state, int x, int y) {
	a=win_width/range_x;
	b=win_height/range_y;
	x=(x-win_width/2)/a*1.0;
	y=(win_height/2-y)/b*1.0;
	mouse_x = x;
	mouse_y = y;

	if (state == GLUT_DOWN){
		if (button == GLUT_LEFT_BUTTON){
			selectedObjByMouse();
		}else if (button == GLUT_RIGHT_BUTTON){
			highlight_flag=0;
		}
	}
}

float perpendicularDistance(point p1,equation e1){
	return (e1.A*p1.x+e1.B*p1.y+e1.C)/sqrt(e1.A*e1.A+e1.B*e1.B);
}

int checkRange(point p1,point p2,float x,float y,float extra){
	point p3,p4,p5,p6,point1;
	equation e1;
	point1.x=x;
	point1.y=y;
	float fct=2.0*highlight_factor;
	float len,cos_angle,sin_angle;
	len=distancePoint(p1,p2);
	e1=eqLine(p1,p2);
	dis=perpendicularDistance(point1,e1);
	cos_angle=(p2.x-p1.x)/len;
	sin_angle=(p2.y-p1.y)/len;
	p3.x=p1.x-fct*sin_angle;
	p4.x=p2.x-fct*sin_angle;
	p5.x=p2.x+fct*sin_angle;
	p6.x=p1.x+fct*sin_angle;
	p3.y=p1.y+fct*cos_angle;
	p4.y=p2.y+fct*cos_angle;
	p5.y=p2.y-fct*cos_angle;
	p6.y=p1.y-fct*cos_angle;
	//	if((fabs(x)+Zero >= (fabs(min(p1.x, p2.x))-extra) && fabs(x)-Zero <= (fabs(max(p1.x, p2.x))+extra)) && fabs(y)+Zero >= (fabs(min(p1.y, p2.y))-extra) && fabs(y)-Zero <= (fabs(max(p1.y, p2.y))+extra))
	if(x+Zero >= min(min(p3.x, p4.x),min(p5.x,p6.x)) && x-Zero <= max(max(p3.x, p4.x),max(p5.x,p6.x)) && y+Zero >= min(min(p3.y, p4.y),min(p5.y,p6.y)) && y-Zero <= max(max(p3.y, p4.y),max(p5.y,p6.y)))
		return 1;
	return 0;
}

void selectedObjByMouse(){
	int i,flag=0,min=world_width;
	for(i=0;i<no_of_proj;i++){
		flag=checkRange(proj_point[i][0],proj_point[i][1],mouse_x,mouse_y,extra);
		if(flag==1){
			if(min>dis){
				highlight_flag=1;
				obj_selected=2;
				index_obj=i;
				min=dis;
			}
		}		
	}
	for(i=0;i<no_of_blocks;i++){
		flag=checkRange(block_point[i][0],block_point[i][1],mouse_x,mouse_y,extra);
		if(flag==1){
			if(min>dis){
				highlight_flag=1;
				obj_selected=1;
				index_obj=i;
				min=dis;
			}
		}		
	}
	for(i=0;i<no_of_mirrors;i++){
		flag=checkRange(mirror_point[i][0],mirror_point[i][1],mouse_x,mouse_y,extra);
		if(flag==1){
			if(min>dis){
				highlight_flag=1;
				obj_selected=0;
				index_obj=i;
				min=dis;
			}
		}		
	}
	return;
}

void selectedObjByKeyBoard(){
	selected_by_key+=1;
	selected_by_key%=(no_of_mirrors+no_of_blocks+no_of_proj);
	if(selected_by_key<no_of_mirrors){
		index_obj=selected_by_key;
		obj_selected=0;
	}else if((selected_by_key-no_of_mirrors)<no_of_blocks){
		index_obj=selected_by_key-no_of_mirrors;
		obj_selected=1;
	}else if((selected_by_key-no_of_mirrors-no_of_blocks)<no_of_proj){
		index_obj=selected_by_key-no_of_mirrors-no_of_blocks;
		obj_selected=2;
	}
}

void update(int value){
	win_width = glutGet((GLenum)GLUT_WINDOW_WIDTH);
	win_height = glutGet((GLenum)GLUT_WINDOW_HEIGHT);
	windowSize();
	glViewport(0, 0, win_width, win_height);
	gluOrtho2D(-range_x/2, range_x/2, -range_y/2, range_y/2);
	glMatrixMode(GL_MODELVIEW);

	if(random_mode==1)
		randomMode();
/*-------------------------To make Lines appear Move------------------*/
	stipple = 0x00ff << value;
	if (value > 8)  
		stipple += 0x00ff >> (8-(value-8));

	value++;
	if (value >= 16) 
		value = 0;  
/*--------------------------------------------------------------------*/
	defineProj();
	defineMirror();
	defineBlock();
	glutPostRedisplay();
	glutTimerFunc(update_speed, update, value);
}

void translate(point **object_point,int index,int x,int y,float *object_cos_angle,float *object_sin_angle,float *object_len){
	object_point[index][0].x+=x*trans_speed;
	object_point[index][1].x+=x*trans_speed;
	object_point[index][0].y+=y*trans_speed;
	object_point[index][1].y+=y*trans_speed;

	if(object_point[index][0].x < ((-world_width)/2 + margin_x)){
		object_point[index][0].x+=world_width-object_len[index]*object_cos_angle[index];
		object_point[index][1].x+=world_width-object_len[index]*object_cos_angle[index];
	}
	if(object_point[index][0].x > world_width/2 - margin_x){
		object_point[index][0].x-=world_width-object_len[index]*object_cos_angle[index];
		object_point[index][1].x-=world_width-object_len[index]*object_cos_angle[index];
	}
	if(object_point[index][0].y < ((-world_height)/2 - margin_y)){
		object_point[index][0].y+=world_height-object_len[index]*object_sin_angle[index];
		object_point[index][1].y+=world_height-object_len[index]*object_sin_angle[index];
	}
	if(object_point[index][0].y > world_height/2 - margin_y){
		object_point[index][0].y-=world_height-object_len[index]*object_sin_angle[index];
		object_point[index][1].y-=world_height-object_len[index]*object_sin_angle[index];
	}
	if(object_point[index][1].x < ((-world_width)/2)+margin_x){
		object_point[index][0].x+=world_width-object_len[index]*object_cos_angle[index];
		object_point[index][1].x+=world_width-object_len[index]*object_cos_angle[index];
	}
	if(object_point[index][1].x > world_width/2 - margin_x){
		object_point[index][0].x-=world_width-object_len[index]*object_cos_angle[index];
		object_point[index][1].x-=world_width-object_len[index]*object_cos_angle[index];
	}
	if(object_point[index][1].y < (-world_height)/2 + margin_y){
		object_point[index][0].y+=world_height-object_len[index]*object_sin_angle[index];
		object_point[index][1].y+=world_height-object_len[index]*object_sin_angle[index];
	}
	if(object_point[index][1].y > world_height/2 - margin_y){
		object_point[index][0].y-=world_height-object_len[index]*object_sin_angle[index];
		object_point[index][1].y-=world_height-object_len[index]*object_sin_angle[index];
	}
}

void resizeObj(point **object_point,int index,int x,int y,float *object_cos_angle,float *object_sin_angle,float *object_len){
	if(x==-1) {   //decrease length
		if(object_len[index]>5.0){
			object_len[index]-=1.0;
		}
	} else if(x==1) {  //increase length
		if(object_len[index]<min(world_height,world_width)/2.0){
			object_len[index]+=1.0;
		}
	}
	float mid_x,mid_y;
	mid_x=(object_point[index][1].x+object_point[index][0].x)/2.0;
	mid_y=(object_point[index][1].y+object_point[index][0].y)/2.0;
	object_point[index][0].x=mid_x-(object_len[index]/2.0)*object_cos_angle[index];
	object_point[index][0].y=mid_y-(object_len[index]/2.0)*object_sin_angle[index];
	object_point[index][1].x=mid_x+(object_len[index]/2.0)*object_cos_angle[index];
	object_point[index][1].y=mid_y+(object_len[index]/2.0)*object_sin_angle[index];
	defineProj();
	defineMirror();
	defineBlock();
	return;
}

void gazeCursorToProjector(){
	int index;
	for(index=0;index<no_of_proj;index++){
		point p1,p2;
		p1.x=proj_src_point[index].x;
		p1.y=proj_src_point[index].y;
		p2.x=mouse_x;
		p2.y=mouse_y;
		float len,cos_angle,sin_angle,mid_x,mid_y;
		mid_x=(proj_point[index][1].x+proj_point[index][0].x)/2.0;
		mid_y=(proj_point[index][1].y+proj_point[index][0].y)/2.0;
		len=distancePoint(p1,p2);
		cos_angle=(p2.x-p1.x)/len;
		sin_angle=(p2.y-p1.y)/len;
		proj_point[index][0].x=mid_x-(proj_len[index]/2.0)*sin_angle;
		proj_point[index][0].y=mid_y+(proj_len[index]/2.0)*cos_angle;
		proj_point[index][1].x=mid_x+(proj_len[index]/2.0)*sin_angle;
		proj_point[index][1].y=mid_y-(proj_len[index]/2.0)*cos_angle;
	}
	defineProj();
}

void randomMode(){
	int choice,i;
	srand(time(NULL));
	for(i=0;i<no_of_proj;i++){
		choice=rand()%6;
		if(choice==0)
			translate(proj_point,i,1,0,proj_cos_angle,proj_sin_angle,proj_len);
		else if(choice==1)
			translate(proj_point,i,0,1,proj_cos_angle,proj_sin_angle,proj_len);
		else if(choice==2)
			translate(proj_point,i,-1,0,proj_cos_angle,proj_sin_angle,proj_len);
		else if(choice==3)
			translate(proj_point,i,0,-1,proj_cos_angle,proj_sin_angle,proj_len);
		else if(choice==4)
			rotate(mirror_point,mirror_cos_angle,mirror_sin_angle,mirror_len,i,1);
		else if(choice==5)
			rotate(mirror_point,mirror_cos_angle,mirror_sin_angle,mirror_len,i,-1);
	}
	for(i=0;i<no_of_mirrors;i++){
		choice=rand()%6;
		if(choice==0)
			translate(mirror_point,i,1,0,mirror_cos_angle,mirror_sin_angle,mirror_len);
		else if(choice==1)
			translate(mirror_point,i,0,1,mirror_cos_angle,mirror_sin_angle,mirror_len);
		else if(choice==2)
			translate(mirror_point,i,-1,0,mirror_cos_angle,mirror_sin_angle,mirror_len);
		else if(choice==3)
			translate(mirror_point,i,0,-1,mirror_cos_angle,mirror_sin_angle,mirror_len);
		else if(choice==4)
			rotate(mirror_point,mirror_cos_angle,mirror_sin_angle,mirror_len,i,1);
		else if(choice==5)
			rotate(mirror_point,mirror_cos_angle,mirror_sin_angle,mirror_len,i,-1);
	}
	for(i=0;i<no_of_blocks;i++){
		choice=rand()%6;
		if(choice==0)
			translate(block_point,i,1,0,block_cos_angle,block_sin_angle,block_len);
		else if(choice==1)
			translate(block_point,i,0,1,block_cos_angle,block_sin_angle,block_len);
		else if(choice==2)
			translate(block_point,i,-1,0,block_cos_angle,block_sin_angle,block_len);
		else if(choice==3)
			translate(block_point,i,0,-1,block_cos_angle,block_sin_angle,block_len);
		else if(choice==4)
			rotate(block_point,block_cos_angle,block_sin_angle,block_len,i,1);
		else if(choice==5)
			rotate(block_point,block_cos_angle,block_sin_angle,block_len,i,-1);
	}
}

void rotate(point **object_point,float *object_cos_angle,float *object_sin_angle,float *object_len,int index,int dir){
	float angle,cos_angle,sin_angle;
	point old_point[2];
	angle=dir*rotate_speed*( PI/180.0);

	cos_angle=cos(angle);
	sin_angle=sin(angle);
	old_point[0].x=object_point[index][0].x;
	old_point[1].x=object_point[index][1].x;
	old_point[0].y=object_point[index][0].y;
	old_point[1].y=object_point[index][1].y;
	/*
	//To rotate about mid-point
	float mid_x,mid_y;
	mid_x=(object_point[index][0].x+object_point[index][1].x)/2.0;
	mid_y=(object_point[index][0].y+object_point[index][1].y)/2.0;
	object_point[index][0].x=mid_x-((mid_x-old_point[0].x)*cos_angle-(mid_y-old_point[0].y)*sin_angle);
	object_point[index][0].y=mid_y-((mid_x-old_point[0].x)*sin_angle+(mid_y-old_point[0].y)*cos_angle);
	object_point[index][1].x=mid_x+((old_point[1].x-mid_x)*cos_angle-(old_point[1].y-mid_y)*sin_angle);
	object_point[index][1].y=mid_y+((old_point[1].x-mid_x)*sin_angle+(old_point[1].y-mid_y)*cos_angle);
	*/
	//To rotate about one point of the two
	object_point[index][1].x=old_point[0].x+((old_point[1].x-old_point[0].x)*cos_angle-(old_point[1].y-old_point[0].y)*sin_angle);
	object_point[index][1].y=old_point[0].y+((old_point[1].x-old_point[0].x)*sin_angle+(old_point[1].y-old_point[0].y)*cos_angle);

	return;
}
