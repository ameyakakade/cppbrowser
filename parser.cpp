#include "parser.h"
#include <iostream>
#include <unordered_set>

enum readingState{
    readingTagStart, readingTag, readingTagContents, outside, readingTagEnd
};

enum dataType{
    none, tag, endTag, textData
};

void htmlParser::parse(std::string input){
    domTree = new treeNode("ROOT", nullptr);
    int state = outside;
    int type = none;
    std::string data;
    treeNode* curr = domTree;
    std::string attributes;

    for(auto letter : input){

        switch (letter){

            case '<':
                state = readingTagStart;
                type = tag;
                break;

            case '/':
                if(state == readingTagStart){
                    type = endTag; 
                }
                break;

            case '>':
                state = readingTagEnd;
                break;

            case ' ':
                if(state == readingTag){
                    state = readingTagContents;
                }

            default:
                if(state == readingTagStart){
                    state = readingTag;
                }else if(state == readingTagEnd){
                    state = outside;
                    type = textData;
                }
        }

        switch (state){

            case readingTagStart: {
                // init data when we start reading smth
                bool check = false;
                for(char c : data) check = check or (c != ' ') and (c != '\n') and (c != '\t');
                if(check){
                    curr->text = data;
                }
                data.clear();
                attributes.clear();
                break;
            }

            case readingTag:
                data += letter;
                break;

            case readingTagContents:
                attributes += letter;
                break;

            case outside:
                data += letter;
                break;

            case readingTagEnd:{
                bool check = false;
                for(char c : attributes) check = check or (c != ' ') and (c != '\n') and (c != '\t');
                if(type == tag){
                    treeNode* temp = new treeNode(data, curr);
                    curr->children.emplace_back(temp);
                    if(check){
                        temp->rawAttributes = attributes;
                    }
                    curr = temp;
                }else if (type == endTag){
                    curr = curr->parentNode;
                }
                data.clear();
                attributes.clear();
                break;
            }

        }

    }

}

void htmlParser::traverse(treeNode* node, int level){
    std::string indent;
    for(int i=0; i<level; i++){
        indent += "  ";
    }
    std::cout << indent << node->name << node->text << node->rawAttributes;
    for(auto property : node->attributes){
        std::cout << " " << property.name << ":" << property.value;
    }
    std::cout << "\n";
    for(auto child : node->children){
        traverse(child, level+1);
    }
}

void htmlParser::inheritCss(treeNode* node){

    // store a pointer to parent node's attributes
    std::vector<cssProperty>* parentCss = &node->parentNode->attributes; 

    // store attributes of self with name and name and index in hashmap
    std::unordered_set<std::string> selfCssAttributesCache;
    for(int i=0; i<node->attributes.size(); i++){
        selfCssAttributesCache.insert(node->attributes[i].name);
    }

    // add all attributes from parent node
    for(cssProperty attribute : *parentCss){
        if(!selfCssAttributesCache.count(attribute.name)){
            // std::cout << "oo i inherit from body" << std::endl;
            node->attributes.push_back(attribute);
        }else{
            // std::cout << "oops i already have it" << std::endl;
        }
    }

    // inherit css for child classes
    for(auto child : node->children){
        inheritCss(child);
    }
}

layoutData htmlParser::calculateLayout(treeNode* node){
    layoutData layout;

    // calculate dimensions for all children
    for(auto child : node->children){
        calculateLayout(child);
    }

    return layout;
}

treeNode::treeNode(std::string input, treeNode* parent){
    name = input;
    parentNode = parent;
}

treeNode::~treeNode(){
    for(auto child : children){
        delete child;
    }
}
