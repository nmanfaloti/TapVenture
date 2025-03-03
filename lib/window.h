typedef struct Window {
    SDL_Window *window;      
    SDL_Renderer *renderer;  
    int width;               
    int height;              
    int InitialWidth;        
    int InitialHeight;       
    int quit;                
    TTF_Font *LargeFont;     
    TTF_Font *MediumFont;    
    TTF_Font *SmallFont;     
    TTF_Font *font;          
    AppState state;          
    double textSpeed;        
} Window;
