#include "MWEditor.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <SDL.h>
#include <math.h>
#include "MWEditorMenu.h"
#include "MWUniversal.h"
#include "MWParts.h"
#include "MWMarbles.h"
#include "MWWorlds.h"
#include "GLTextures.h"
#include "GLDraw.h"
using namespace std;

/// 'Tors
MWEditor::MWEditor() {
    defaults();
}
MWEditor::~MWEditor() {
}
void MWEditor::defaults() {
    menu_item_index = MENU_BASE;
    simulating = false;
    show_menu = true;
    part_template_index = 0;
    marble_template_index = 0;
    prop_template_index = 0;
    creation_type = Part;
}

/// Methods
// Initialization
void MWEditor::initialize() {
    initialize_sdl();
    initialize_opengl();
    load_all_textures();
    initialize_part_templates();
    initialize_marble_templates();
    initialize_prop_templates();
    initialize_menu();

    world = make_world(WORLD_PLANET_1);

    /// TEMP
    MWRobot player;
    player.x = WORLD_CENTER_X + 400;
    player.y = WORLD_CENTER_Y - world.planet_radius - 100;
    player.vx = 10;
    world.robots.push_back(player);
    ///
}
void MWEditor::initialize_sdl() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) quit();            // Try to initialize sdl subsystems
    const SDL_VideoInfo* info = SDL_GetVideoInfo();     // Get the machine's video information
    Uint8 bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if(SDL_SetVideoMode(SCREEN_W, SCREEN_H, bpp, SDL_OPENGL) == 0) quit();
}
void MWEditor::initialize_opengl() {
    glClearColor(.5f, .5f, .5f, 1.f);
    glViewport(0, 0, SCREEN_W, SCREEN_H);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_W, SCREEN_H, 0.0f, -1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                   // when texture area is large, bilinear filter the original
}
void MWEditor::initialize_part_templates() {
    initialize_part_template(PART_TRACK_1);
    initialize_part_template(PART_TRACK_ENTRANCE_END_1);
    initialize_part_template(PART_TRACK_CURVE45_1);
    initialize_part_template(PART_TRACK_LOOP_SMALL_QUARTER_1);
    initialize_part_template(PART_TRACK_LOOP_MEDIUM_EIGHTH_1);
    initialize_part_template(PART_SWITCH_1);
    initialize_part_template(PART_TRIGGER_1);
}
void MWEditor::initialize_part_template(void (*part_generator) (MWPart *part)) {
    MWEditorPartTemplate part_template;
    part_template.generator = part_generator;
    part_template.instance = make_part(part_generator);
    part_templates.push_back(part_template);
}
void MWEditor::initialize_prop_templates() {
    initialize_prop_template(PROP_SUPPORT_1_SHORT);
    initialize_prop_template(PROP_SUPPORT_1_LONG);
    initialize_prop_template(PROP_SUPPORT_1_TOP);
}
void MWEditor::initialize_prop_template(void (*prop_generator) (MWProp *prop)) {
    MWEditorPropTemplate prop_template;
    prop_template.generator = prop_generator;
    prop_template.instance = make_prop(prop_generator);
    prop_templates.push_back(prop_template);
}
void MWEditor::initialize_marble_templates() {
    initialize_marble_template(MARBLE_STEEL);
}
void MWEditor::initialize_marble_template(void (*marble_generator) (MWMarble *marble)) {
    MWEditorMarbleTemplate marble_template;
    marble_template.generator = marble_generator;
    marble_template.instance = make_marble(marble_generator);
    marble_templates.push_back(marble_template);
}
void MWEditor::initialize_menu() {
    menu.defaults();
}
// Running
void MWEditor::run() {
    initialize();                       // Initialize the system
    int dt;                             // Set up timing variables
    Uint32 t0;
    Uint32 t1 = SDL_GetTicks() + DT;
    running = true;                     // Set the "running" flag to true
    while (running) {                   // Begin the main loop

        /// TEMP
        // Center the view on the player robot, and adjust the angle
        /*if (simulating) {
            world.view.x = world.player.x - SCREEN_W / 2;
            world.view.y = world.player.y - SCREEN_H / 2;
            world.view.angle = 270.f - 57.2958f * atan2(world.player.y - WORLD_CENTER_Y, world.player.x - WORLD_CENTER_X);
        }*/
        ///

        gl_clear_transformations();     //   Clear the matrix transformations
        update();                       //   Update all models
        glPushMatrix();
        world.view_transformations();   //   Set up the transformation matrix for the world's view
        handle_events();                //   Handle SDL events, like mouse and keyboard presses
        draw_world();                   //   Draw the world
        glPopMatrix();
        draw_menu();

        // Blit everything to the screen
        SDL_GL_SwapBuffers();

        t0 = SDL_GetTicks();            //   Moderate the loop timing
        dt = t1 - t0;
        if (dt < 0) dt = 0;
        SDL_Delay(dt);
        t1 += DT;
    }
    quit();                             // Run the exiting scripts
}
void MWEditor::update() {
    menu.update();
    world.update_globalization();
    if (simulating) world.update();
}
void MWEditor::update_menu() {

    menu_item_index = MENU_BASE;

    // Check to see if have an item selected
    if (selection.selected) {
        switch (selection.type) {
        case Part:
            if (keys.key_lctrl || keys.key_rctrl)
                menu_item_index = MENU_PART_LINK;
            else
                menu_item_index = MENU_PART;
            break;
        case Prop:
            menu_item_index = MENU_PROP;
            break;
        case Marble:
            menu_item_index = MENU_MARBLE;
            break;
        }
    }

    // Check for the various shift menus
    else if (keys.key_lshift || keys.key_lshift) {
        switch (creation_type) {
        case Part:
            menu_item_index = MENU_SHIFT_PART;
            break;
        case Prop:
            menu_item_index = MENU_SHIFT_PROP;
            break;
        case Marble:
            menu_item_index = MENU_SHIFT_MARBLE;
            break;
        }
    }

    if (show_menu) menu.open(menu_item_index);
}
void MWEditor::draw_world() {
    static MWPart *part;
    static MWProp *prop;
    static MWPropConnectionVertex *prop_vertex;
    static MWMarble *marble;
    static MWPath *path;
    static MWPartEntrance *entrance;
    static MWPartConnection *connection;
    static MWPathVertex *path_vertex;

    // Render the world
    world.draw();

    // Draw prop bounding circles & path vertices
    for (int i = 0; i < (int)world.props.size(); i ++) {
        prop = &world.props[i];

        // Bounding circle
        GLDraw_circle(prop->x, prop->y, prop->r, 4.f, .4f, 1.f, 0.f, .4f);

        // Connection vertices
        for (int ii = 0; ii < (int)prop->connection_vertices.size(); ii ++) {
            prop_vertex = &prop->connection_vertices[ii].vertex_global;
            GLDraw_line(prop_vertex->x, prop_vertex->y - 10,
                        prop_vertex->x, prop_vertex->y + 10,
                        2.f, 1.f, 0.f, 1.f, 0.5f);
            GLDraw_line(prop_vertex->x - 10, prop_vertex->y,
                        prop_vertex->x + 10, prop_vertex->y,
                        2.f, 1.f, 0.f, 1.f, 0.5f);
        }
    }

    // Draw part bounding circles & connection vertices
    for (int i = 0; i < (int)world.parts.size(); i ++) {
        part = &world.parts[i];

        // Bounding circle
        GLDraw_circle(part->x, part->y, part->r, 4.f, .4f, 1.f, 0.f, .4f);

        // Path vertices
        if (part->path_set_index != -1) {
            for (int ii = 0; ii < (int)part->path_sets[part->path_set_index].paths_global.size(); ii ++) {
                path = &part->path_sets[part->path_set_index].paths_global[ii];
                path->draw();

                // Entrances
                MWPartPathSet *path_set = &part->path_sets[part->path_set_index];
                for (int entrance_i = 0; entrance_i < (int)path_set->entrances.size(); entrance_i ++) {
                    entrance = &path_set->entrances[entrance_i];
                    path = &path_set->paths_global[entrance->path_index];
                    path_vertex = &path->vertices[entrance->vertex_index];
                    GLDraw_circle(path_vertex->x, path_vertex->y, entrance->radius, 6, 1.f, .5f, 0.f, .6f);
                    GLDraw_circle(path_vertex->x, path_vertex->y, entrance->inner_radius, 3, 1.f, 0.f, 0.f, .5f);
                }

                // Connections
                for (int connection_i = 0; connection_i < (int)path_set->connections.size(); connection_i ++) {
                    connection = &path_set->connections[connection_i];
                    path = &path_set->paths_global[connection->path_index[0]];
                    path_vertex = &path->vertices[connection->vertex_index[0]];
                    GLDraw_circle(path_vertex->x, path_vertex->y, CONNECTION_RADIUS, 7, .3f, .3f, 1.f, .8f);

                    /// TODO: Posibly it could draw a line between the connected vertices - most of the time it would
                    ///       probably be either invisible or appear as a dot, because of the closeness of their proximity, but it
                    ///       may just be visually helpful for confirmation of connections
                }
            }
        }

        // Connection vertices
        for (int ii = 0; ii < (int)part->connection_vertices.size(); ii ++) {
            prop_vertex = &part->connection_vertices[ii].vertex_global;
            GLDraw_line(prop_vertex->x, prop_vertex->y - 10,
                        prop_vertex->x, prop_vertex->y + 10,
                        2.f, 1.f, 0.f, 1.f, 0.5f);
            GLDraw_line(prop_vertex->x - 10, prop_vertex->y,
                        prop_vertex->x + 10, prop_vertex->y,
                        2.f, 1.f, 0.f, 1.f, 0.5f);
        }
    }

    // Draw circles around pathed marbles
    for (int i = 0; i < (int)world.marbles.size(); i ++) {
        marble = &world.marbles[i];
        if (marble->pathed) GLDraw_circle(marble->x, marble->y, marble->r, 4.f, 1.f, 0.f, 0.f);
    }

    // Draw arrows representing links between components
    int link_index;
    for (int i = 0; i < (int)world.parts.size(); i ++) {
        for (int ii = 0; ii < (int)world.parts[i].link_part_indices.size(); ii ++) {
            link_index = world.parts[i].link_part_indices[ii];
            GLDraw_arrow(
                world.parts[i].x, world.parts[i].y, world.parts[link_index].x, world.parts[link_index].y,
                32, 1, .5, 0., .5);
        }
    }

    // Draw a box around the first component that is found to be under the mouse or being placed
    static float x, y;
    static float r;
    static bool found;
    if (selection.selected) {
        switch (selection.type) {
        case Part:
            x = selected_part()->x;
            y = selected_part()->y;
            r = selected_part()->r;
            break;
        case Marble:
            x = selected_marble()->x;
            y = selected_marble()->y;
            r = selected_marble()->r;
            break;
        case Prop:
            x = selected_prop()->x;
            y = selected_prop()->y;
            r = selected_prop()->r;
            break;
        }
        GLDraw_line(x-r, y-r, x+r, y-r, 5.f, 0.f);
        GLDraw_line(x-r, y+r, x+r, y+r, 5.f, 0.f);
        GLDraw_line(x-r, y-r, x-r, y+r, 5.f, 0.f);
        GLDraw_line(x+r, y-r, x+r, y+r, 5.f, 0.f);
    } else {
        found = false;
        for (int i = 0; i < (int)world.parts.size(); i ++) {
            static float dx;
            static float dy;
            x = world.parts[i].x;
            y = world.parts[i].y;
            dx = mouse.x - x;
            dy = mouse.y - y;
            r = world.parts[i].r;
            if (dx*dx + dy*dy < r*r) {
                GLDraw_line(x-r, y-r, x+r, y-r, 5.f);
                GLDraw_line(x-r, y+r, x+r, y+r, 5.f);
                GLDraw_line(x-r, y-r, x-r, y+r, 5.f);
                GLDraw_line(x+r, y-r, x+r, y+r, 5.f);
                found = true;
                break;
            }
        }
        if (!found) {
            for (int i = 0; i < (int)world.marbles.size(); i ++) {
                static float dx;
                static float dy;
                x = world.marbles[i].x;
                y = world.marbles[i].y;
                dx = mouse.x - x;
                dy = mouse.y - y;
                r = world.marbles[i].r;
                if (dx*dx + dy*dy < r*r) {
                    GLDraw_line(x-r, y-r, x+r, y-r, 5.f);
                    GLDraw_line(x-r, y+r, x+r, y+r, 5.f);
                    GLDraw_line(x-r, y-r, x-r, y+r, 5.f);
                    GLDraw_line(x+r, y-r, x+r, y+r, 5.f);
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            for (int i = 0; i < (int)world.props.size(); i ++) {
                static float dx;
                static float dy;
                x = world.props[i].x;
                y = world.props[i].y;
                dx = mouse.x - x;
                dy = mouse.y - y;
                r = world.props[i].r;
                if (dx*dx + dy*dy < r*r) {
                    GLDraw_line(x-r, y-r, x+r, y-r, 5.f);
                    GLDraw_line(x-r, y+r, x+r, y+r, 5.f);
                    GLDraw_line(x-r, y-r, x-r, y+r, 5.f);
                    GLDraw_line(x+r, y-r, x+r, y+r, 5.f);
                    found = true;
                    break;
                }
            }
        }
    }

    // If we are linking components, then draw an arrow from the selected component to the mouse
    if (selection.selected && selection.type == Part && (keys.key_lctrl || keys.key_rctrl)) {
        GLDraw_arrow(selected_part()->x, selected_part()->y, mouse.x, mouse.y, 32, 0.f, 1.f, 1.f);
    }

    // Draw a preview of the selected component if the control button is being pressed
    if (keys.key_lshift || keys.key_rshift) {
        glColor4f(1.f, 1.f, 1.f, .5f);
        switch (creation_type) {
        case Part:
            part = &part_templates[part_template_index].instance;
            for (int i = 0; i < (int)part->graphics_back.size(); i ++)
                part->graphics_back[i].draw(mouse.x, mouse.y);
            for (int i = 0; i < (int)part->graphics_fore.size(); i ++)
                part->graphics_fore[i].draw(mouse.x, mouse.y);
            break;
        case Marble:
            marble = &marble_templates[marble_template_index].instance;
            //marble->graphic.draw(mouse.x, mouse.y);
            marble->draw(mouse.x, mouse.y);
            break;
        case Prop:
            prop = &prop_templates[prop_template_index].instance;
            for (int i = 0; i < (int)prop->graphics.size(); i ++)
                prop->graphics[i].draw(mouse.x, mouse.y);
            break;
        }
        glColor4f(1.f, 1.f, 1.f, 1.f);
    }

    // Draw a border around the world
    GLDraw_line(0, 0, WORLD_W, 0, 30);
    GLDraw_line(WORLD_W, 0, WORLD_W, WORLD_H, 30);
    GLDraw_line(WORLD_W, WORLD_H, 0, WORLD_H, 30);
    GLDraw_line(0, WORLD_H, 0, 0, 30);
}
void MWEditor::draw_menu() {
    menu.draw(mouse.screen_x, mouse.screen_y);
}
void MWEditor::quit() {
    free_all_textures();
    SDL_Quit();
    exit(0);
}
void MWEditor::gl_clear_transformations() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// Editing
int MWEditor::under_mouse(MWWorldObjectType object_type) {
    static float dx;
    static float dy;
    static float r;
    static float x, y;

    switch (object_type) {
    case Part:
        for (int i = 0; i < (int)world.parts.size(); i ++) {
            x = world.parts[i].x;
            y = world.parts[i].y;
            dx = mouse.x - x;
            dy = mouse.y - y;
            r = world.parts[i].r;
            if (dx*dx + dy*dy < r*r) {
                return i;
            }
        }
        break;
    case Prop:
        for (int i = 0; i < (int)world.props.size(); i ++) {
            x = world.props[i].x;
            y = world.props[i].y;
            dx = mouse.x - x;
            dy = mouse.y - y;
            r = world.props[i].r;
            if (dx*dx + dy*dy < r*r) {
                return i;
            }
        }
        break;
    case Marble:
        for (int i = 0; i < (int)world.marbles.size(); i ++) {
            x = world.marbles[i].x;
            y = world.marbles[i].y;
            dx = mouse.x - x;
            dy = mouse.y - y;
            r = world.marbles[i].r;
            if (dx*dx + dy*dy < r*r) {
                return i;
            }
        }
        break;
    }
    return -1;
}
MWPart *MWEditor::selected_part() {
    return (selection.selected && selection.type == Part) ? &world.parts[selection.index] : 0;
}
MWProp *MWEditor::selected_prop() {
    return (selection.selected && selection.type == Prop) ? &world.props[selection.index] : 0;
}
MWMarble *MWEditor::selected_marble() {
    return (selection.selected && selection.type == Marble) ? &world.marbles[selection.index] : 0;
}
float MWEditor::angle_to_center(float x, float y) {
    return 57.2958f * atan2(WORLD_CENTER_Y - y, WORLD_CENTER_X - x) - 90;
}
// SDL event handling
void MWEditor::handle_events() {
    static SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_KEYDOWN:
            handle_key_down(&event.key.keysym);
            break;
        case SDL_KEYUP:
            handle_key_up(&event.key.keysym);
        case SDL_MOUSEBUTTONDOWN:
            handle_mouse_down(&event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            handle_mouse_up(&event.button);
        case SDL_MOUSEMOTION:
            handle_mouse_motion(&event.motion);
            break;
        case SDL_QUIT:
            running = false;
            break;
        }
        update_menu();
    }
}
void MWEditor::handle_key_down(SDL_keysym *keysym) {

    float dx = 256.f;
    float da = 45.f * .5f;
    float ds = 1.3f;
    switch(keysym->sym) {
    case SDLK_SPACE:
        simulating = !simulating;
        break;
    case SDLK_UP:
        world.view.y -= dx;
        break;
    case SDLK_DOWN:
        world.view.y += dx;
        break;
    case SDLK_LEFT:
        world.view.x -= dx;
        break;
    case SDLK_RIGHT:
        world.view.x += dx;
        break;
    case SDLK_a:    // Move character RIGHT
        if (simulating) {
            world.player()->vx -= 10;
        }
        break;
    case SDLK_d:    // Move character LEFT
        if (simulating) {
            world.player()->vx += 10;
        }
        break;
    case SDLK_z:
        if (selection.selected && selection.type == Part) {
            selected_part()->angle -= da;
            world.update_part_globalization(selection.index);
            break;
        }
        if (selection.selected && selection.type == Prop) {
            selected_prop()->angle -= da;
            world.update_prop_globalization(selection.index);
            break;
        }
        world.view.angle += da;
        break;
    case SDLK_x:
        if (selection.selected && selection.type == Part) {
            selected_part()->angle += da;
            world.update_part_globalization(selection.index);
            break;
        }
        if (selection.selected && selection.type == Prop) {
            selected_prop()->angle += da;
            world.update_prop_globalization(selection.index);
            break;
        }
        world.view.angle -= da;
        break;
    case SDLK_EQUALS:
        world.view.scale *= ds;
        break;
    case SDLK_MINUS:
        world.view.scale /= ds;
        break;
    case SDLK_LSHIFT:
        keys.key_lshift = true;
        break;
    case SDLK_RSHIFT:
        keys.key_rshift = true;
        break;
    case SDLK_LCTRL:
        keys.key_lctrl = true;
        break;
    case SDLK_RCTRL:
        keys.key_rctrl = true;
        break;
    case SDLK_LALT:
        keys.key_lalt = true;
        break;
    case SDLK_RALT:
        keys.key_ralt = true;
        break;
    case SDLK_TAB:
        if (show_menu) {
            show_menu = false;
            menu.close();
        } else {
            show_menu = true;
            update_menu();
        }
        break;
    case SDLK_BACKSPACE:
        if (selection.selected && selection.type == Part)
            selected_part()->link_part_indices.clear();
        break;
    case SDLK_DELETE:
        if (selection.selected) {
            switch (selection.type) {
            case Part:
                for (int i = 0; i < (int)selected_part()->marbles.size(); i ++)
                    selected_part()->marble(i)->pathed = false;
                world.remove_part(selection.index);
                selection.deselect();
                break;
            case Prop:
                world.remove_prop(selection.index);
                selection.deselect();
                break;
            case Marble:
                world.remove_marble(selection.index);
                selection.deselect();
                break;
            }
        }
        break;
    case SDLK_ESCAPE:
        running = false;
        break;
    default:
        break;
    }
}
void MWEditor::handle_key_up(SDL_keysym *keysym) {
    switch(keysym->sym) {
    case SDLK_LSHIFT:
        keys.key_lshift = false;
        break;
    case SDLK_RSHIFT:
        keys.key_rshift = false;
        break;
    case SDLK_LCTRL:
        keys.key_lctrl = false;
        break;
    case SDLK_RCTRL:
        keys.key_rctrl = false;
        break;
    case SDLK_LALT:
        keys.key_lalt = false;
        break;
    case SDLK_RALT:
        keys.key_ralt = false;
        break;
    default:
        break;
    }
}
void MWEditor::handle_mouse_down(SDL_MouseButtonEvent *button) {
    float da = 15.f;
    float ds = 1.3f;
    int id, part_id, marble_id, prop_id;
    MWWorldObjectType object_types[] = {Part, Prop, Marble};
    int object_types_count = 3;
    switch(button->button) {
    case SDL_BUTTON_LEFT:
        mouse.button_left = true;

        // If the shift button is down, create a new part
        if (keys.key_lshift || keys.key_rshift) {
            switch (creation_type) {
            case Part:
                part_id = world.add_part(part_templates[part_template_index].generator, mouse.x, mouse.y);
                selection.select(Part, part_id);
                break;
            case Marble:
                marble_id = world.add_marble(marble_templates[marble_template_index].generator, mouse.x, mouse.y);
                selection.select(Marble, marble_id);
                break;
            case Prop:
                prop_id = world.add_prop(prop_templates[prop_template_index].generator, mouse.x, mouse.y);
                selection.select(Prop, prop_id);
                break;
            }
            break;
        }

        // Check to see if there is a component under the mouse
        for (int i = 0; i < object_types_count; i ++) {
            id = under_mouse(object_types[i]);
            if (id > -1) {
                selection.select(object_types[i], id);
                break;
            }
        }

        break;
    case SDL_BUTTON_RIGHT:
        mouse.button_right = true;

        if (keys.key_lshift || keys.key_rshift) {
            /// TODO: Make this better... I mean, I guess it works - it's just kinda nasty
            if (creation_type      == Part) {creation_type    = Prop; }
            else if (creation_type == Prop) { creation_type   = Marble; }
            else if (creation_type == Marble) { creation_type = Part; }
        }

        break;
    case SDL_BUTTON_WHEELUP:
        if (keys.key_lshift || keys.key_rshift) {
            switch (creation_type) {
            case Part:
                part_template_index ++;
                if (part_template_index >= (int)part_templates.size()) part_template_index = 0;
                break;
            case Prop:
                prop_template_index ++;
                if (prop_template_index >= (int)prop_templates.size()) prop_template_index = 0;
                break;
            case Marble:
                marble_template_index ++;
                if (marble_template_index >= (int)marble_templates.size()) marble_template_index = 0;
                break;
            }
            break;
        }
        if (selection.selected && selection.type == Part) {
            selected_part()->angle -= da * .5f;
            world.update_part_globalization(selection.index);
            break;
        }
        if (selection.selected && selection.type == Prop) {
            selected_prop()->angle -= da * .5f;
            world.update_prop_globalization(selection.index);
            break;
        }
        if (mouse.button_right)
            world.view.angle -= da;
        else
            world.view.scale *= ds;
        break;
    case SDL_BUTTON_WHEELDOWN:
        if (keys.key_lshift || keys.key_rshift) {
            switch (creation_type) {
            case Part:
                part_template_index --;
                if (part_template_index < 0) part_template_index = (int)part_templates.size() - 1;
                break;
            case Prop:
                prop_template_index --;
                if (prop_template_index < 0) prop_template_index = (int)prop_templates.size() - 1;
                break;
            case Marble:
                marble_template_index --;
                if (marble_template_index < 0) marble_template_index = (int)marble_templates.size() - 1;
                break;
            }
            break;
        }
        if (selection.selected && selection.type == Part) {
            selected_part()->angle += da *.5f;
            world.update_part_globalization(selection.index);
            break;
        }
        if (selection.selected && selection.type == Prop) {
            selected_prop()->angle += da *.5f;
            world.update_prop_globalization(selection.index);
            break;
        }
        if (mouse.button_right)
            world.view.angle += da;
        else
            world.view.scale /= ds;
        break;
    }
}
void MWEditor::handle_mouse_up(SDL_MouseButtonEvent *button) {
    switch(button->button) {
    case SDL_BUTTON_LEFT:
        mouse.button_left = false;

        // Check to see if a link should be made
        if (selection.selected && selection.type == Part && (keys.key_lctrl || keys.key_rctrl)) {
            int part_id = under_mouse(Part);
            if (part_id != -1 && &world.parts[part_id] != selected_part()) {
                bool link = true;
                for (int i = 0; i < (int)selected_part()->link_part_indices.size(); i ++)
                    if (selected_part()->link_part_indices[i] == part_id) link = false;
                if (link) selected_part()->link_part_indices.push_back(part_id);
            }
        }

        // Check to see if a part should look for path connections
        if (selection.selected && selection.type == Part)
            world.parts[selection.index].connect_by_proximity();

        // Make sure to deselect anything that may have been selected
        selection.deselect();
        break;
    case SDL_BUTTON_RIGHT:
        mouse.button_right = false;
        break;
    }
}
void MWEditor::handle_mouse_motion(SDL_MouseMotionEvent *motion) {

    // Update the mouse' globalized coordinates
    mouse.screen_x = motion->x;
    mouse.screen_y = motion->y;
    globalize_mouse();

    // If both mouse buttons are down, drag the view around
    if (mouse.button_left && mouse.button_right) {
        world.view.x += -(1.f / world.view.scale) * (cos(world.view.angle * 2.f * 3.14159265f / 360.f) * motion->xrel + sin(world.view.angle * 2.f * 3.14159265f / 360.f) * motion->yrel);
        world.view.y -= -(1.f / world.view.scale) * (sin(world.view.angle * 2.f * 3.14159265f / 360.f) * motion->xrel - cos(world.view.angle * 2.f * 3.14159265f / 360.f) * motion->yrel);
        return;
    }

    // If we are linking a component to another component...
    if (mouse.button_left && selection.selected && selection.type == Part && (keys.key_lctrl || keys.key_rctrl)) {

        // Check to see if there is a component under the mouse
        int part_id = under_mouse(Part);
        if (part_id > -1 && &world.parts[part_id] != selected_part()) {
            mouse.x = world.parts[part_id].x;
            mouse.y = world.parts[part_id].y;
        }
        return;
    }

    // If a part is being placed, try to snap to the vertices of other components
    if (mouse.button_left && selection.selected) {
        static float snap_r = 20.f;
        static float snap_r2 = snap_r*snap_r;
        static MWPart *part;
        static MWProp *prop;
        static path_vec *path0;
        static path_vec *path1;
        static MWPathVertex *vert0;
        static MWPathVertex *vert1;
        static MWPropConnectionVertex *con_vert0;
        static MWPropConnectionVertex *con_vert1;
        static float dx, dy;
        static float r;
        static float angle;
        static bool snapped;

        switch (selection.type) {
        case Part:

            /// TODO: Clean up this god-aweful snapping mechanism, you sick bastard
            // Snap to other parts' path vertices
            snapped = false;
            path0 = selected_part()->path_set_global();
            for (int i_path0 = 0; i_path0 < (int)path0->size(); i_path0 ++) {
                for (int i_vert0 = 0; i_vert0 < (int)path0->at(i_path0).vertices.size(); i_vert0 ++) {
                    vert0 = &path0->at(i_path0).vertices[i_vert0];
                    for (int i_part1 = 0; i_part1 < (int)world.parts.size(); i_part1 ++) {
                        part = &world.parts[i_part1];
                        if (part == selected_part()) continue;

                        dx = selected_part()->x - part->x;
                        dy = selected_part()->y - part->y;
                        r = selected_part()->r + part->r + snap_r;
                        if (dx*dx + dy*dy > r*r) continue;
                
                        path1 = part->path_set_global();
                        for (int i_path1 = 0; i_path1 < (int)path1->size(); i_path1 ++) {
                            for (int i_vert1 = 0; i_vert1 < (int)path1->at(i_path1).vertices.size(); i_vert1 ++) {
                                vert1 = &path1->at(i_path1).vertices[i_vert1];

                                dx = vert1->x - vert0->x;
                                dy = vert1->y - vert0->y;

                                if (dx*dx + dy*dy < snap_r2) {
                                    selected_part()->x += dx;
                                    selected_part()->y += dy;
                                    snapped = true;
                                }

                                if (snapped) break;
                            }
                            if (snapped) break;
                        }
                        if (snapped) break;
                    }
                    if (snapped) break;
                }
                if (snapped) break;
            }

            // Snap to prop connection vertices
            if (!snapped) {
                for (int i_vert0 = 0; i_vert0 < (int)selected_part()->connection_vertices.size(); i_vert0 ++) {
                    con_vert0 = &selected_part()->connection_vertices[i_vert0].vertex_global;
                    for (int i_prop = 0; i_prop < (int)world.props.size(); i_prop ++) {
                        prop = &world.props[i_prop];

                        dx = selected_part()->x - prop->x;
                        dy = selected_part()->y - prop->y;
                        r = selected_part()->r + prop->r + snap_r;
                        if (dx*dx + dy*dy > r*r) continue;

                        for (int i_vert1 = 0; i_vert1 < (int)prop->connection_vertices.size(); i_vert1 ++) {
                            con_vert1 = &prop->connection_vertices[i_vert1].vertex_global;

                            dx = con_vert1->x - con_vert0->x;
                            dy = con_vert1->y - con_vert0->y;

                            if (dx*dx + dy*dy < snap_r2) {
                                selected_part()->x += dx;
                                selected_part()->y += dy;
                                snapped = true;
                            }
                            if (snapped) break;
                        }
                        if (snapped) break;
                    }
                    if (snapped) break;
                }
            }

            // Un-snap if the mouse has moved far enough away from the contact points
            if (snapped) {
                dx = mouse.x - selected_part()->x;
                dy = mouse.y - selected_part()->y;
                if (dx*dx + dy*dy > snap_r2) snapped = false;
            }

            // Move the component or marble to the mouse' position
            if (!snapped) {
                selected_part()->x = mouse.x;
                selected_part()->y = mouse.y;
            }

            // If the alt-key is being held, make the part parallel to the surface of the planet
            if (keys.key_lalt || keys.key_ralt) {
                angle = angle_to_center(selected_part()->x, selected_part()->y);
                selected_part()->angle = angle;
            }

            // Make sure the path's for this component get globalized
            world.update_part_globalization(selection.index);

            // End this rediculously huge switch entry
            break;

        case Prop:

            snapped = false;
            /// Again, you suck for making such an ugly chunk of code.  Either fix it, or jump off a bridge, please
            for (int i_vert0 = 0; i_vert0 < (int)selected_prop()->connection_vertices.size(); i_vert0 ++) {
                con_vert0 = &selected_prop()->connection_vertices[i_vert0].vertex_global;

                // Snap to other props' connection vertices
                for (int i_prop = 0; i_prop < (int)world.props.size(); i_prop ++) {
                    prop = &world.props[i_prop];
                    if (selected_prop() == prop) continue;

                    dx = selected_prop()->x - prop->x;
                    dy = selected_prop()->y - prop->y;
                    r = selected_prop()->r + prop->r + snap_r;
                    if (dx*dx + dy*dy > r*r) continue;

                    for (int i_vert1 = 0; i_vert1 < (int)prop->connection_vertices.size(); i_vert1 ++) {
                        con_vert1 = &prop->connection_vertices[i_vert1].vertex_global;

                        dx = con_vert1->x - con_vert0->x;
                        dy = con_vert1->y - con_vert0->y;

                        if (dx*dx + dy*dy < snap_r2) {
                            selected_prop()->x += dx;
                            selected_prop()->y += dy;
                            snapped = true;
                        }
                        if (snapped) break;
                    }
                    if (snapped) break;
                }
                if (snapped) break;

                // Snap to part connection vertices
                for (int i_part = 0; i_part < (int)world.parts.size(); i_part ++) {
                    part = &world.parts[i_part];
                    
                    dx = selected_prop()->x - part->x;
                    dy = selected_prop()->y - part->y;
                    r = selected_prop()->r + part->r + snap_r;
                    if (dx*dx + dy*dy > r*r) continue;

                    for (int i_vert1 = 0; i_vert1 < (int)part->connection_vertices.size(); i_vert1 ++) {
                        con_vert1 = &part->connection_vertices[i_vert1].vertex_global;

                        dx = con_vert1->x - con_vert0->x;
                        dy = con_vert1->y - con_vert0->y;

                        if (dx*dx + dy*dy < snap_r2) {
                            selected_prop()->x += dx;
                            selected_prop()->y += dy;
                            snapped = true;
                        }
                        if (snapped) break;
                    }
                    if (snapped) break;
                }
                if (snapped) break;
            }

            // Un-snap if the mouse has moved far enough away from the contact points
            if (snapped) {
                dx = mouse.x - selected_prop()->x;
                dy = mouse.y - selected_prop()->y;
                if (dx*dx + dy*dy > snap_r2) snapped = false;
            }

            // Move the prop to the mouse' position
            if (!snapped) {
                selected_prop()->x = mouse.x;
                selected_prop()->y = mouse.y;
            }

            // If the alt-key is being held, make the part parallel to the surface of the planet
            if (keys.key_lalt || keys.key_ralt) {
                angle = angle_to_center(selected_prop()->x, selected_prop()->y);
                selected_prop()->angle = angle;
            }

            // Update the prop's globalized vertices
            world.update_prop_globalization(selection.index);
            break;

        case Marble:
            selected_marble()->x = mouse.x;
            selected_marble()->y = mouse.y;
            selected_marble()->vx = 
            selected_marble()->vy = 0.f;
            break;
        }
    }
}
void MWEditor::globalize_mouse() {

    // Get the mouse world coordinates
    static GLint viewport[4];
    static GLdouble modelview[16], projection[16];
    static GLfloat wx, wy;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    wx = (GLfloat)mouse.screen_x;
    wy = (GLfloat)(viewport[3] - mouse.screen_y);

    static double x, y, z;
    gluUnProject(wx, wy, 0, modelview, projection, viewport, &x, &y, &z);
    mouse.x = (float)x;
    mouse.y = (float)y;
}