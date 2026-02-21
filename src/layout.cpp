#include "layout.h"
#include <iostream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, Color> stringToColorMap = {

        {"WHITE",      GetColor(0xFFFFFFFF)},
        {"SILVER",     GetColor(0xC0C0C0FF)},
        {"GRAY",       GetColor(0x808080FF)},
        {"BLACK",      GetColor(0x000000FF)},
        
        // Reds & Pinks
        {"RED",        GetColor(0xFF0000FF)},
        {"CRIMSON",    GetColor(0xDC143CFF)},
        {"PINK",       GetColor(0xFFC0CBFF)},
        {"HOTPINK",    GetColor(0xFF69B4FF)},
        {"MAGENTA",    GetColor(0xFF00FFFF)},

        // Oranges & Yellows
        {"ORANGE",     GetColor(0xFFA500FF)},
        {"CORAL",      GetColor(0xFF7F50FF)},
        {"GOLD",       GetColor(0xFFD700FF)},
        {"YELLOW",     GetColor(0xFFFF00FF)},
        {"BEIGE",      GetColor(0xF5F5DCFF)},

        // Greens
        {"GREEN",      GetColor(0x008000FF)},
        {"LIME",       GetColor(0x00FF00FF)},
        {"OLIVE",      GetColor(0x808000FF)},
        {"DARKGREEN",  GetColor(0x006400FF)},
        {"TEAL",       GetColor(0x008080FF)},

        // Blues
        {"AQUA",       GetColor(0x00FFFFFF)},
        {"SKYBLUE",    GetColor(0x87CEEBFF)},
        {"BLUE",       GetColor(0x0000FFFF)},
        {"NAVY",       GetColor(0x000080FF)},
        {"ROYALBLUE",  GetColor(0x4169E1FF)},
        {"MIDNIGHTBLUE", GetColor(0x191970FF)},

        // Purples & Browns
        {"PURPLE",     GetColor(0x800080FF)},
        {"VIOLET",     GetColor(0xEE82EEFF)},
        {"INDIGO",     GetColor(0x4B0082FF)},
        {"BROWN",      GetColor(0xA52A2AFF)},
        {"MAROON",     GetColor(0x800000FF)}
};

// body and root node
void layoutTree::makeLayoutTree(treeNode* node, layoutNode* parentLayout){

    // create a layout node
    layoutNode* currentLayoutNode = new layoutNode;

    // add its parent layout node
    currentLayoutNode->parent = parentLayout;

    // link it to its origin dom node
    currentLayoutNode->originNode = node;

    // add it to children of parent layout node
    parentLayout->children.push_back(currentLayoutNode);

    /* filling data in layout nodes */

    // lambda function for filling margin and padding data
    auto fillArray = [&](float* array, const std::string& identifier){
        std::vector<std::string> suffix = {"-top", "-bottom", "-right", "-left"};
        for(int i=0; i<4; i++) array[i] = convertStringToPx(node->style[node->cssPropertyIndexCache[identifier+(suffix[i])]].value);
    };

    // filling margin and padding
    fillArray(currentLayoutNode->margin, "margin");
    fillArray(currentLayoutNode->padding, "padding");

    // filling the display type 
    currentLayoutNode->display = returnDisplayType(node->style[node->cssPropertyIndexCache["display"]].value);

    // filling the background color 
    currentLayoutNode->backgroundColor = convertStringToColor(node->style[node->cssPropertyIndexCache["background-color"]].value);

    // now we have to recurse and add layout nodes for all children of the treenode which will be linked to current layout node
    for(auto child : node->children){
        makeLayoutTree(child, currentLayoutNode);
    }

}

void layoutTree::traverse(layoutNode* node, int level){
    std::string indent;
    for(int i=0; i<level; i++){
        indent += "   ";
    }
    std::cout << indent << node->originNode->name << " " << std::endl;
    std::cout << " " << " height:" << node->height;
    std::cout << " " << "  width:" << node->width;
    std::cout << " " << "      x:" << node->x;
    std::cout << " " << "      y:" << node->y;

    std::cout << "\n";
    for(auto child : node->children){
        traverse(child, level+1);
    }
}

float layoutTree::convertStringToPx(std::string& input){
    std::string data;
    std::string type;

    int state = 0;
    for(char c : input){
        if(state == 0){
            if( c>47 && c<58 || c==46){
                data += c;
            }else if(c == ' '){
                // do nothing
            }else{
                state = 1;
                type += c;
            }
        }else if(state == 1){
            if(c == ' ') break;
            type += c;
        }
    }

    float value;

    try{
        value = stof(data);
    }catch(std::invalid_argument){
        std::cout << "Invalid CSS in \"" << input << "\"" << std::endl;
        value = 16;
    }

    float multiplier = 1;

    if(type == "em") multiplier = emToPx;
    if(type == "px") multiplier = 1;

    return value*multiplier;
}

displayType layoutTree::returnDisplayType(std::string& input){
    std::string temp;
    int state = 0;
    for(char c : input){
        if(c>65 && c<91) {
            temp += (c + 32);
            state = 1;
        }else if(c>96 && c<123){
            temp += c;
            state = 1;
        }else{
            if(state == 1) break;
        }
    }

    displayType value;

    if(temp == "block"){
        value = displayType::displayBlock;
    }else if(temp == "inline"){
        value = displayType::displayInline;
    }else{
        std::cout << "Invalid CSS in \"" << input << "\"" << std::endl;
        value = displayType::displayBlock;
    }
    return value;

}

// top, bottom, right, left;

float layoutTree::calculateLayoutPass(layoutNode* node, float availableWidth){
    float nodeWidth;
    float nodeHeight = 0;

    // node width is available width minus left and right margin
    nodeWidth = availableWidth - (node->margin[2] + node->margin[3])*scale;
    
    node->x = cursorX + node->margin[3]*scale;
    node->y = cursorY + node->margin[0]*scale;

    cursorX = node->x + node->padding[3]*scale;
    cursorY = node->y + node->padding[0]*scale;

    float newAvailableWidth = nodeWidth - (node->padding[2] + node->padding[3])*scale;

    for(auto child : node->children){
        nodeHeight += calculateLayoutPass(child, newAvailableWidth);
    }

    nodeHeight += (node->padding[0] + node->padding[1])*scale;

    cursorX = node->x - node->margin[3]*scale;
    cursorY = node->y + nodeHeight + node->margin[1]*scale;

    node->height = nodeHeight;
    node->width  = nodeWidth;

    return nodeHeight + (node->margin[0] + node->margin[1])*scale;
}


Color layoutTree::convertStringToColor(std::string& input){
    std::string temp;
    int state = 0;
    for(char c : input){
        if(c>65 && c<91) {
            temp += c;
            state = 1;
        }else if(c>96 && c<123){
            temp += (c - 32);
            state = 1;
        }else{
            if(state == 1) break;
        }
    }

    Color value = WHITE;
    
    if(stringToColorMap.count(temp) == 1) value = stringToColorMap[temp];

    return value;
}
