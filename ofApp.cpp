/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	read_flag = 0;
	isdfs = false;
	isDFS = 0;
	isBFS = 0;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS(0);
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			BFS(0);
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {
	//ofBackground(0, 0, 0, 0);
	char str[100];
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	if (read_flag == 1)
	{ 
		int y = 0, x = 0;
		for (i = 0; i < HEIGHT; i++)
		{
			for (j = 0; j < WIDTH; j++)
			{
				if (input_for_draw[i][j] == '|')
				{
					ofDrawRectangle(x, y, 5, 20);
					x += 5;
				}
				else if (input_for_draw[i][j] == '-')
				{
					ofDrawRectangle(x, y, 20, 5);
					x += 20;
				}
				else if (input_for_draw[i][j] == ' ')
				{
					if (j % 2 == 0)
						x += 5;
					else
						x += 20;
				}
				else if (input_for_draw[i][j] == '+')
				{
					ofDrawRectangle(x, y, 5, 5);
					x += 5;
				}
				else //숫자 들어가있을 경우 공백과 동일하게 처리 <- 필요 없음
				{
					if (j % 2 == 0)
						x += 5;
					else
						x += 20;
				}
			}
			x = 0;
			if (i % 2 == 0)
				y += 5;
			else
				y += 20;
		}
	}

	if (isDFS == 1)
	{
		//ofSetColor(200);
		//ofSetLineWidth(5);
		if (isOpen)
		{
			dfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}

	if (isBFS == 1)
	{
		if (isOpen)
		{
			bfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}

	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		char tmpname[1000];
		strcpy(tmpname, fileName.c_str());
		//string fileName = "maze0.maz";
		printf("file name is %s\n", tmpname);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다

			// 파일을 읽어 WIDTH와 HEIGHT 구하기
			HEIGHT = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
			{
				string line = *it;
				WIDTH = line.size();
				HEIGHT++;
			}

			//printf("%d %d\n", HEIGHT, WIDTH);

			//구한 HEIGHT와 WIDTH 이용하여 input에 메모리 할당
			input_for_draw = (char**)malloc(sizeof(char*) * HEIGHT);
			for (int i = 0; i < HEIGHT; i++)
				input_for_draw[i] = (char*)malloc(sizeof(char) * WIDTH);


			rooms = (int**)malloc(sizeof(int*) * HEIGHT);
			for (int i = 0; i < HEIGHT; i++)
				rooms[i] = (int*)malloc(sizeof(int) * WIDTH);

			//오류 방지를 위해 -1로 초기화 (방이 아닌 부분에는 -1이 있어야 함)
			for (int i = 0; i < HEIGHT; i++)
				for (int j = 0; j < WIDTH; j++)
					rooms[i][j] = -1;
	
			//각 방에 번호 부여
			int tmpcnt = 0;
			for (int i = 1; i < HEIGHT; i += 2)
			{
				for (int j = 1; j < WIDTH; j += 2)
				{
					rooms[i][j] = tmpcnt;
					tmpcnt++;
				}
			}

			roomnum = (HEIGHT - 1) / 2 * (WIDTH - 1) / 2; //vertex의 개수

			maze_row = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
			{
				string line = *it;
				for (maze_col = 0; maze_col < WIDTH; maze_col++)
				{
					input_for_draw[maze_row][maze_col] = line[maze_col];
				}
				maze_row++;
			}

			//debug
			/*printf("input_for_draw\n");
			for (int i = 0; i < HEIGHT; i++)
			{
				for (int j = 0; j < WIDTH; j++)
				{
					printf("%c", input_for_draw[i][j]);
				}
				printf("\n");
			}*/

			//debug
			/*printf("input\n");
			for (int i = 0; i < HEIGHT; i++)
			{
				for (int j = 0; j < WIDTH; j++)
				{
					printf("%c", input[i][j]);
				}
				printf("\n");
			}*/

			//DEBUG
			//printf("height:%d / width:%d / roomnum:%d\n", HEIGHT, WIDTH, roomnum);

			int room1, room2;

			//희소행렬로 표현한 인접행렬
			adj_cnt = 0;
			for (int i = 1; i < HEIGHT; i += 2)
			{
				for (int j = 1; j < WIDTH; j += 2)
				{
					if (input_for_draw[i][j + 1] == ' ')
					{
						room1 = rooms[i][j];
						room2 = rooms[i][j + 2];
						adj[adj_cnt].room1 = room1;
						adj[adj_cnt].room2 = room2;
						adj_cnt++;
					}
					if (input_for_draw[i + 1][j] == ' ')
					{
						room1 = rooms[i][j];
						room2 = rooms[i + 2][j];
						adj[adj_cnt].room1 = room1;
						adj[adj_cnt].room2 = room2;
						adj_cnt++;
					}
				}
			}

			//debug
			/*printf("adjacency\n");
			for (int i = 0; i < roomnum; i++)
			{
				for (int j = 0; j < roomnum; j++)
				{
					printf("%d ", adjacency[i][j]);
				}
				printf("\n");
			}*/

			//debug
			/*printf("adj\n");
			for (int i = 0; i < adj_cnt; i++)
			{
				printf("room1: %d, room2: %d\n", adj[i].room1, adj[i].room2);
			}*/

			read_flag = 1;
			if (isDFS == 1) isDFS = 0; //한 미로에서 dfs 후 다른 미로를 열었을 때 바로 dfs되는것 방지
			if (isBFS == 1) isBFS = 0;
		}

		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
	printf("read complete\n");
}

void ofApp::freeMemory() {
	//TO DO
	// malloc한 memory를 free해주는 함수
	int i;

	for (i = 0; i < HEIGHT; i++)
		free(input_for_draw[i]);
	free(input_for_draw);

	free(visited);
	free(push_trail);
	free(pop_trail);
	printf("free complete\n");
}

bool ofApp::DFS(int v)//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 (3주차)
	if (isBFS == 1) isBFS = 0;
	visited = (int*)calloc(roomnum, sizeof(int));
	push_trail = (int*)calloc(roomnum, sizeof(int));
	push_cnt = 0;
	pop_trail = (int*)calloc(roomnum, sizeof(int));
	pop_cnt = 0;

	stack<int> s;
	s.push(v);
	visited[v] = 1;

	push_trail[push_cnt] = v;
	push_cnt++;
	//첫 번째 노드 스택에 넣고 VISITED 처리

	while (!s.empty())
	{
		if (s.top() == roomnum - 1)
		{
			isDFS = 1;
			return true; //마지막 방에 도착하면 return
		}

		int flag = 0; int room;

		
		for (int i = 0; i < roomnum; i++)
		{
			for (int j = 0; j < adj_cnt; j++) //adj 탐색하여,
			{
				if ((adj[j].room1 == i && adj[j].room2 == s.top()) || (adj[j].room1 == s.top() && adj[j].room2 == i)) //adj에 [i, s.top()]이나 [s.top(), i]가 존재하는 경우
				{
					if (visited[i] == 0)
					{
						flag = 1;
						room = i;
					}
				}
			}
		}

		if (flag == 1)
		{
			s.push(room); //PUSH하면 초록
			push_trail[push_cnt] = room;
			push_cnt++;
			visited[room] = 1;
		}
		else
		{
			int poproom = s.top(); //pop하면 빨간색 
			s.pop();
			pop_trail[pop_cnt] = poproom;
			pop_cnt++;
		}
	}
	return false; //마지막 방에 도착하지 못한 경우(잘못된 경우)
}

void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차)
	ofSetColor(0, 255, 0);
	int x, y;
	int i;
	for (i = 0; i < push_cnt; i++)
	{
		x = push_trail[i] / ((WIDTH - 1) / 2);
		y = push_trail[i] % ((WIDTH - 1) / 2);
		x = 2 * x + 1;
		y = 2 * y + 1;
		ofDrawCircle(15 + 12.5 * (y - 1), 15 + 12.5 * (x - 1), 5);
	}

	ofSetColor(255, 0, 0);
	for (i = 0; i < pop_cnt; i++)
	{
		x = pop_trail[i] / ((WIDTH - 1) / 2);
		y = pop_trail[i] % ((WIDTH - 1) / 2);
		x = 2 * x + 1;
		y = 2 * y + 1;
		ofDrawCircle(15 + 12.5 * (y - 1), 15 + 12.5 * (x - 1), 5);
	}
}

bool ofApp::BFS(int v)//DFS탐색을 하는 함수
{
	if (isDFS == 1) isDFS = 0;

	visited = (int*)calloc(roomnum, sizeof(int));
	parent = (int*)calloc(roomnum, sizeof(int));
	path = (int*)calloc(roomnum, sizeof(int));
	path_cnt = 0;
	push_trail = (int*)calloc(roomnum, sizeof(int));
	push_cnt = 0;

	queue<int> q;
	q.push(v);
	visited[v] = 1;

	push_trail[push_cnt] = v;
	push_cnt++;

	while (!q.empty())
	{
		int i, j;
		int flag = 0; int room;
		room = q.front();
		q.pop();
		//printf("pop: %d\n", room);

		if (room == roomnum - 1)
		{
			//path 구하기 0부터 n까지가 아니라 n부터 0까지 경로 저장됨
			int par;
			path[0] = roomnum - 1;
			path_cnt++;
			while (1)
			{
				par = parent[path[path_cnt - 1]];
				path[path_cnt] = par;
				path_cnt++;
				
				if (par == 0) break;
			}

			isBFS = 1;
			return true;
		}
		for (i = 0; i < roomnum; i++)
		{
			for (j = 0; j < adj_cnt; j++)
			{
				if ((adj[j].room1 == i && adj[j].room2 == room) || (adj[j].room1 == room && adj[j].room2 == i))
				{
					if (visited[i] == 0)
					{
						visited[i] = 1;
						q.push(i);

						parent[i] = room;

						push_trail[push_cnt] = i;
						push_cnt++;
						//printf("push: %d\n", i);
					}
				}
			}
		}
	}
	return false; 
}

void ofApp::bfsdraw()
{
	ofSetColor(150, 250, 150);
	int x, y;
	int i;
	for (i = 0; i < push_cnt; i++)
	{
		x = push_trail[i] / ((WIDTH - 1) / 2);
		y = push_trail[i] % ((WIDTH - 1) / 2);
		x = 2 * x + 1;
		y = 2 * y + 1;
		ofDrawCircle(15 + 12.5 * (y - 1), 15 + 12.5 * (x - 1), 5);
	}
	ofSetColor(200, 50, 150);
	for (i = path_cnt-1; i >= 0; i--)
	{
		x = path[i] / ((WIDTH - 1) / 2);
		y = path[i] % ((WIDTH - 1) / 2);
		x = 2 * x + 1;
		y = 2 * y + 1;
		ofDrawCircle(15 + 12.5 * (y - 1), 15 + 12.5 * (x - 1), 5);
	}
}
