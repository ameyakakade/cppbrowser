#include "parser.h"
#include <iostream>
#include <unordered_set>

enum readingState{
    reading, outside, readingend
};

void htmlParser::parse(std::string input){
    domTree = new treeNode("ROOT", nullptr);
    int state = outside;
    std::string word;
    std::string data;
    treeNode* curr = domTree;
    std::vector<std::string> attributes;

    for(auto letter : input){

        // convert this into a parse -> set state -> then act on the state type thing

        if(letter=='<'){
            // write the data to the current node
            // set state to reading tag
            curr->data = data;
            state = reading;
            word = "";
            data = "";
        }else if(state == reading && word == "" && letter == '/'){
            // if end tag is found set state to reading end and move up the dom tree
            curr = curr->parentNode;
            state = readingend;
        }else if(letter=='>' && state==reading){
            // if tag is ended and we were reading a tag make a new node in the tree and move curr to it
            state = outside;
            treeNode* temp = new treeNode(word, curr);
            curr->children.emplace_back(temp);
            curr = temp;
        }else if(state==reading){
            // when reading html tag
            word += letter;
        }else if(state == outside){
            // when reading data
            data += letter;
        }

    }

}

void htmlParser::traverse(treeNode* node, int level){
    std::string indent;
    for(int i=0; i<level; i++){
        indent += '\t';
    }
    std::cout << indent << node->name << node->data << std::endl;
    for(auto property : node->attributes){
        std::cout << property.name << " " << property.value << std::endl;
    }
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
            std::cout << "oo i inherit from body" << std::endl;
            node->attributes.push_back(attribute);
        }else{
            std::cout << "oops i already have it" << std::endl;
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
