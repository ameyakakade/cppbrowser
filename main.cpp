#include "raylib/include/raylib.h"
#include "url.h"
#include <sys/socket.h>
#include "parser.h"

int main(){

    // converting address to ip and getting html from server
    std::string test = "http://127.0.0.1/index.html";
    urlReader testReader;
    testReader.read(test);
    std::string header, body;
    testReader.request(header, body);

    // parsing the html to make a dom tree
    htmlParser parser;
    parser.parse(body); // passing in the html
                        //

    // adding some basic attributes to the html node
    treeNode* htmlNode;
    for(auto node : parser.domTree->children){
        if(node->name == "html") htmlNode = node;
    }

    // adding css property to html node
    // these will only be inherited by nodes in body not in head

    htmlNode->style.push_back(cssProperty{"color", "black", true});
    
    // inherit css properties only for the nodes in body 

    parser.parseAttributes(parser.domTree);

    for(auto node : htmlNode->children){
        if(node->name == "body") parser.inheritCss(node);
    }

    parser.traverse(parser.domTree, 0);

    // rendering 

    // SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    // InitWindow(800, 600, "raylib [core] example - basic window");
    //
    // while (!WindowShouldClose())
    // {
    //     int boxPositionY = GetMouseWheelMove();
    //     BeginDrawing();
    //         ClearBackground(BLACK);
    //         DrawText(body.c_str(), 190, 200, 20, LIGHTGRAY);
    //     EndDrawing();
    // }
    //
    // CloseWindow();

    return 0;
}
