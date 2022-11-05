/*******************************************************************************************
*
*   combination of core_custom_frame_control.c and core_3d_camera_free.c
*   https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux building using these steps for now
*
********************************************************************************************/
#include "p2k1.h"


// TODO: must put this in gamestate or some other header
// todo: gca_loaded is a placeholder for some part of the game state that manages the current active controllers and their configs
// GameInputs may also be put in a ControllerInputs layer 
/*
 * p2k1_process_inputs()
 * writes: p1_inputs, p2_inputs
 * reads libs: raylib 
 */
void p2k1_process_inputs(GameInputs *p1_input, GameInputs *p2_input, bool gca_loaded)
{
    PollInputEvents(); // raylib poll keyboard + mouse

    ControllerStatus adapter_buffer[4]; // gca

    if(gca_loaded)
    {
        gca::Process(adapter_buffer);

        //std::cout << "+++++++++++++++" << std::endl;
        std::cout << gca::RawData() << std::endl;
        /*
        std::cout << adapter_buffer[0] << std::endl;
        std::cout << adapter_buffer[1] << std::endl;
        std::cout << adapter_buffer[2] << std::endl;
        std::cout << adapter_buffer[3] << std::endl;\
        */


        p1_input->mainStickHorizontal = adapter_buffer[0].mainStickHorizontal;
        p1_input->mainStickVertical   = adapter_buffer[0].mainStickVertical;
        p1_input->cStickHorizontal    = adapter_buffer[0].cStickHorizontal; 
        p1_input->cStickVertical      = adapter_buffer[0].cStickVertical;
        p1_input->triggerResult       = adapter_buffer[0].triggerL - adapter_buffer[0].triggerR;
            
        p2_input->mainStickHorizontal = adapter_buffer[1].mainStickHorizontal;
        p2_input->mainStickVertical   = adapter_buffer[1].mainStickVertical;
        p2_input->cStickHorizontal    = adapter_buffer[1].cStickHorizontal; 
        p2_input->cStickVertical      = adapter_buffer[1].cStickVertical;
        p2_input->triggerResult       = adapter_buffer[1].triggerL - adapter_buffer[1].triggerR;

        
    }
    else // TODO: get game inputs from keyboard
    {
        //if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        //if (IsKeyPressed(KEY_UP)) targetFPS += DEBUG_FRAME_INCRIMENT;
        //else if (IsKeyPressed(KEY_DOWN)) targetFPS -= DEBUG_FRAME_INCRIMENT;

        //if (targetFPS < DEBUG_FRAME_INCRIMENT) targetFPS = DEBUG_FRAME_INCRIMENT;

    }




}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "raylib barebones fighting engine ");

    // try to init GCA+
    bool gca_loaded = gca::Setup();

    // timekeeping 
    double previousTime = GetTime();    // Previous time measure
    double currentTime = 0.0;           // Current time measure
    double updateDrawTime = 0.0;        // Update + Draw time
    double waitTime = 0.0;              // Wait time (if target fps required)
    float deltaTime = 0.0f;             // Frame time (Update + Draw + Wait time)

    // gamestate
    GameState gs;
    GameRenderState rs;
    GameInputs p1_input;
    GameInputs p2_input;
    
    int targetFPS = 60;                 // Our initial target fps
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        p2k1_process_inputs(&p1_input, &p2_input, gca_loaded);

        //std::cout << p2_input.triggerResult << std::endl;
        p2k1_advance_game_state(&p1_input, &p2_input, &gs);

        //fix16_print(gs.p2_rad);
        //fgl_vec3_print(gs.p1_tip);


        p2k1_advance_game_render_state(&gs, &rs);

        //std::cout << rs.camera_position.x << rs.camera_position.y << rs.camera_position.z << std::endl;

        // Draw
        //----------------------------------------------------------------------------------
        p2k1_render_frame(&gs, &rs);

        currentTime = GetTime();
        updateDrawTime = currentTime - previousTime;
        
        if (targetFPS > 0)          // We want a fixed frame rate
        {
            waitTime = (1.0f/(float)targetFPS) - updateDrawTime;
            if (waitTime > 0.0) 
            {
                WaitTime((float)waitTime);
                currentTime = GetTime();
                deltaTime = (float)(currentTime - previousTime);
            }
        }
        else deltaTime = updateDrawTime;    // Framerate could be variable

        previousTime = currentTime;
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    gca::Stop();          // stop gca

    return 0;
}
