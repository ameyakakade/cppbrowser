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

    for(auto letter : input){
        if(letter=='<'){
            curr->data = data;
            state = reading;
            word = "";
            data = "";
        }else if(state == reading && word == "" && letter == '/'){
            curr = curr->parentNode;
            state = readingend;
        }else if(letter=='>' && state==reading){
            state = outside;
            treeNode* temp = new treeNode(word, curr);
            curr->children.emplace_back(temp);
            curr = temp;
        }else if(state==reading){
            word += letter;
        }else if(state == outside){
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
