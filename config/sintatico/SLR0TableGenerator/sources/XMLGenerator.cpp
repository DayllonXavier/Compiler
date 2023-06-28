#include <string>
#include "XMLGenerator.hpp"

XMLGenerator::XMLGenerator(LR0Generator _lr0generator)
{
    lr0generator = _lr0generator;
    for(int i=0; i< lr0generator.states.size(); i++)
    {
        alreadyDrawn.push_back(false);
    }
}
XMLGenerator::~XMLGenerator()
{
}

void XMLGenerator::print_header()
{
    cout << "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<mxfile host=\"app.diagrams.net\" modified=\"2023-06-02T18:50:27.750Z\" agent=\"5.0 (X11)\" etag=\"s__y9iIHghFVF5K_Mr9C\" version=\"20.8.10\" type=\"google\">\n\
  <diagram name=\"Page-1\" id=\"OsU3y5K4XvkoWrooalSU\">\n\
    <mxGraphModel dx=\"1731\" dy=\"2557\" grid=\"1\" gridSize=\"10\" guides=\"1\" tooltips=\"1\" connect=\"1\" arrows=\"1\" fold=\"1\" page=\"1\" pageScale=\"1\" pageWidth=\"850\" pageHeight=\"1100\" math=\"0\" shadow=\"0\">\n\
      <root>\n\
        <mxCell id=\"0\" />\n\
        <mxCell id=\"1\" parent=\"0\" />\n\
";
}

void XMLGenerator::print_footer()
{
    cout << "\n\
      </root>\n\
    </mxGraphModel>\n\
  </diagram>\n\
</mxfile>\n\
";
}

void XMLGenerator::print_list_title(string id, string value, string fillColor, int x, int y, int width ,int height)
{
cout << "\n\
<!-- List Title -->\n\
<mxCell id=\"" << id << "\" \n\
        value=\"" << value << "\" \n\
    style=\"swimlane;fontStyle=0;childLayout=stackLayout;horizontal=1;startSize=26;horizontalStack=0;resizeParent=1;resizeParentMax=0;resizeLast=0;collapsible=1;marginBottom=0;align=center;fontSize=14;shadow=0;sketch=0;fillColor=" << fillColor << ";resizeWidth=0;\" \n\
    vertex=\"1\" \n\
    parent=\"1\">\n\
    <mxGeometry x=\"" << x << "\" y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\" as=\"geometry\" />\n\
</mxCell>\n\
";
}


void XMLGenerator::print_list_element(string id, string value, string fillColor, string parent, int y, int width ,int height)
{
    cout << "\n\
<!-- List Element -->\n\
<mxCell id=\"" << id << "\" \n\
        value=\"" << value << "\" \n\
    style=\"text;strokeColor=default;fillColor=" << fillColor << ";spacingLeft=4;spacingRight=4;overflow=hidden;rotatable=0;points=[[0,0.5],[1,0.5]];portConstraint=eastwest;fontSize=12;fontStyle=1;resizeWidth=0;\" \n\
    vertex=\"1\" \n\
    parent=\"" << parent << "\">\n\
    <mxGeometry y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\" as=\"geometry\" />\n\
</mxCell>\n\
";
}
void XMLGenerator::print_arrow(string id, string value, string source, string target)
{
//entryX=-0.014;entryY=0.186;entryDx=0;entryDy=0;entryPerimeter=0;\" \n
    cout << "\n\
<!-- ARROW -->\n\
<mxCell id=\"" << id << "\" \n\
    value=\"&lt;b&gt;" << value << "&lt;/b&gt;\" \n\
    style=\"edgeStyle=none;rounded=0;orthogonalLoop=1;jettySize=auto;html=1;fontSize=14;\" \n\
    edge=\"1\" \n\
    parent=\"1\" \n\
    source=\"" << source << "\" \n\
    target=\"" << target << "\">\n\
    <mxGeometry relative=\"1\" as=\"geometry\" />\n\
</mxCell>\n\
";
}

void XMLGenerator::draw_state(State state, int x, int y, bool title_only, int parent)
{
    if(title_only)
    {
    print_list_title(   string("State_").append(to_string(state.number)).append("_from_").append(to_string(parent)), 
                        string("Estado ").append(to_string(state.number)), 
                        "#FFFF66", x, y, global_width, 26);
        return;
    }

    string color = "#BCC75A";
    if(state.number == 0) color = "#999999";
    else if(state.reduce()) color = "#EA6B66";
    else if(state.rules.size() > 8) color = "#66FF66";

    print_list_title(   string("State_").append(to_string(state.number)), 
                        string("Estado ").append(to_string(state.number)), 
                        color , x, y, global_width, 26+30*state.rules.size());
    int i=0;
    for(auto rule : state.rules)
    {
        print_list_element( string("State_").append(to_string(state.number)).append("_ruleIndex_").append(to_string(i)),
                            string("(").append(to_string(rule.number)).append(") ").append(rule.left).append(" → ").append(rule.right),
                            color, string("State_").append(to_string(state.number)), 26+30*i, global_width, 30);
        i++;
    }
}

int XMLGenerator::recursive_draw(State state, int x, int y, bool title_only, int parent, int depth)
{
    //if(!title_only) cout << "FULLY DRAWING STATE " << state.number << endl;
    draw_state(state, x, y, title_only, parent);
    if(!title_only) alreadyDrawn[state.number] = true;

    int new_y = y;
    if(title_only){
        y = y + 26 + 30;
        return y;
    }else{
        new_y = y + 26+30*state.rules.size() + 30;
        if(state.transitions.size() == 0) y = new_y;
    }
    //TRANSITIONS ORDER BY INSERTION
    for( auto transition : state.transitions)
    {
    //    int next_state = state.transitions.at(element);
    //    pair<string, int> transition(element,next_state);
    //}
    ////TRANSITIONS ORDER BY VALUE
    //for(int i=0; i<lr0generator.states.size(); i++)//Disgusting hack to get the correct order (map ordered by value)
    //{
    //    for(auto transition : state.transitions)
    //    {
    //        if(transition.second == i)
    //        {
                bool title_only = false;

                string target = string("State_").append(to_string(transition.second));
                bool isBacklink = transition.second <= state.number;

                if(isBacklink) title_only = true;
                if(alreadyDrawn[transition.second]) title_only = true;
                //if(lr0generator.states[transition.second].depth != depth + 1) title_only = true; //enforce depth

                if(title_only) target = target.append("_from_").append(to_string(state.number));
                y = recursive_draw(lr0generator.states[transition.second], x + global_width + 100, y, title_only, state.number, depth+1);


                print_arrow(string("arrow_").append(to_string(state.number)).append("_to_").append(to_string(transition.second)), 
                        transition.first, 
                        string("State_").append(to_string(state.number)),
                        target);
    //        }
    //    }
    }
    y = max(y,new_y);
    return y;
}
