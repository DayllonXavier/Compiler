#ifndef XMLGenerator_hpp
#define XMLGenerator_hpp

using namespace std;

#include <iostream>
#include <string>

#include "LR0Generator.hpp"

class XMLGenerator {
    public:
        XMLGenerator(LR0Generator lr0generator);
        ~XMLGenerator();

        LR0Generator lr0generator;
        int recursion_depth;
        void print_header();
        void print_footer();
        void print_list_title(string id, string value, string fillColor, int x, int y, int width ,int height);
        void print_list_element(string id, string value, string fillColor, string parent, int y, int width ,int height);
        void print_arrow(string id, string value, string source, string target);

        void draw_state(State state, int x, int y, bool title_only, int parent);
        int recursive_draw(State state, int x, int y, bool title_only, int parent, int depth);

        int global_width = 260;
        vector<bool> alreadyDrawn;

};
#endif

