/* 
tuilib.h 

*/
#define OK     1
#define CANCEL 2



typedef enum { label, button, checkbox, radiobutton, simpletext, multitext, numbox, listbox,rectangle } tgui_Object_type;

typedef struct listitem {
	
	char *caption;
	int li_id;
	struct listitem *next;
	struct listitem *prev;
	
} tgui_Listitem;


typedef struct menuitem {
	
	char *caption;           // text to dispay in submenu for this item
	int menu_item_id;        // auto generated ID to be used in providing a return value on selected menu
	struct menuitem *next;   // auto created pointer to next sub menu item
	struct menuitem *prev;   // auto created pointer to previous sub menu item
	void (*fp) (void*);
}  tgui_Menuitem;



typedef struct menu {
	
	char *caption;            // text displayed on the main menu
    int menu_id;              // sequential index auto calculated in order of menu additions
    int mi_maxwidth;          // enternally calculated with each add.
	struct menu *next;        // auto created pointer to next menu in the list
	struct menu *prev;        // auto created pointer to previous menu in the list
	tgui_Menuitem *firstmi;   // auto created pointer to the first submenu item (NULL if none)
	tgui_Menuitem *lastmi;    // auto created pointer to the last submenu item (NULL if none)
	void (*fp) (void*);       // pointer to function to run if menu item selected
		
} tgui_Menu;



//   tgui_Obj data type 
//   this struct defines the parameters needed to craeate, process and destroy window objects
typedef struct object  {
	int x;                    // x position of object relative to x1 of window
	int y;                    // y position of object relative to y1 of window
	int width;                // width of object, not used for all object types
	int height;               // height of object, not used for all object types (numbox->max)
	unsigned id;              // auto generated id to object, used in get_objdata to retreive data
	int color;                // color of text for object
	char *caption;	          // text to display (not used for all objects)
	char *data;               // data string stored for object
	unsigned state;           // for future use (numbox->min)
	unsigned topid;           // top id in list box
    tgui_Object_type objtype; // defines object type see enum types above
	tgui_Listitem *firstitem; // used for list box - link to list of items
	tgui_Listitem *lastitem;  // used for list box - link to list of items
	struct object *next;      // auto created pointer to next object in screen, or NULL if none
	void (*fp)(void *);       // pointer to function to execute for this object, not used for all objects 
	                          // function pointed to should be defined in client code and void * is typically a *tgui_Window
} tgui_Obj;



//   tgui_Window data type 
//   this struct defines the parameters needed to craeate, process and close windows

typedef struct {
	int x1;                // x position of top left corner 0 to 34
	int y1;                // y position of the top left corner 0 to 39
	int x2;                // x position of the bottom right corner x1+5 to 39
	int y2;                // y position of the botton left corner y1+4 to 24
	unsigned parent;       // 0=not parent 1=parent - if not parent screen in footprint will be saved in scrnchar, scrncolor
	unsigned quitflag;     // 1=quit 0=do no quit - used by processmenu and processobjects to quit handling loop
	char *title;           // title of window, can be "\0" for no title
	char *scrnchar;        // pointer to dynamically allocated buffer to hold char screen data when parent =0
	char *scrncolr;        // pointer to dynamically allocated buffer to hold screen color data when parent =0
	int color;             // color of window text 0 to 16
	tgui_Obj *firstobj;    // auto created pointer to first object in the window object list
	tgui_Obj *lastobj;     // auto created pointer to last object in the window object list
	tgui_Menu *firstmenu;  // auto created pointer to the first menu in the window menu (NULL if none)
	tgui_Menu *lastmenu;   // auto created pointer to the last menu in the window menu (NULL if none)
		
} tgui_Window;



tgui_Window *newwindow(int x1, int y1, int x2, int y2, int color, unsigned parent, char* title);
void closewindow(tgui_Window* win);
void msgbox(char *msg);
void inputbox(char *caption, char *buf, unsigned size);

void newobject(int x, int y,int width,int height,int color, unsigned state, char *caption, char *data, 
               tgui_Object_type type, tgui_Window *win, void (*fp)(void *) );
void showobj(tgui_Obj *obj, tgui_Window *win);
void showallobjects (tgui_Window *win);
void addmenu(tgui_Window *win, char *caption, void (*fp)(void *) );
void showmenu(tgui_Window *win);

int processmenu(tgui_Window *win);
void addmenuitem(tgui_Window *win, int menuid, char *caption, void (*fp) (void*));

void process_checkbox(tgui_Window *win, tgui_Obj *obj);
char process_simpletext(tgui_Window *win, tgui_Obj *obj);
void process_listbox(tgui_Window *win, tgui_Obj *obj);
char process_numbox(tgui_Window *win, tgui_Obj *obj);

void processobj(tgui_Window *win, tgui_Obj *obj);
char *get_objdata(tgui_Window *w, unsigned id);
int get_objstate(tgui_Window *w, unsigned id);
tgui_Obj *get_obj(tgui_Window *w, unsigned id);

void add_li(tgui_Obj *obj, char *caption);


