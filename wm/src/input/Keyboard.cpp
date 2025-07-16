#include <LKeyboardKeyEvent.h>
#include <LSessionLockManager.h>
#include <LLauncher.h>
#include <linux/input-event-codes.h>
#include <LCompositor.h>
#include "Keyboard.h"
#include "../utils/Global.h"
#include "../scene/Scene.h"
#include "../utils/Settings.h"

Keyboard::Keyboard(const void *params) noexcept : LKeyboard(params)
{
    /* Key press repeat rate */
    setRepeatInfo(32, 500);

    /* Keymap sent to clients and used by the compositor, check the LKeyboard class or XKB doc */
    setKeymap(
        nullptr,  // Rules
        nullptr,  // Model
        nullptr,  // Layout
        nullptr); // Options
}

void Keyboard::keyEvent(const LKeyboardKeyEvent &event)
{

    /* Launches a session lock client */
    if (sessionLockManager()->state() != LSessionLockManager::Locked
        && event.keyCode() == KEY_POWER
        && event.state() == LKeyboardKeyEvent::Released)
        LLauncher::launch(SETTINGS_SESSION_LOCK_CLIENT);

    const bool LEFT_META  { isKeyCodePressed(KEY_LEFTMETA)  };
    const bool LEFT_SHIFT { isKeyCodePressed(KEY_LEFTSHIFT) };
    const bool LEFT_ALT   { isKeyCodePressed(KEY_LEFTALT)   };
    const bool LEFT_CTRL  { isKeyCodePressed(KEY_LEFTCTRL)  };

    if (event.state() == LKeyboardKeyEvent::Released) {

        // Launch terminal
       if (LEFT_META && event.keyCode() == KEY_ENTER) {
           LLauncher::launch("kitty --detach");
       }

        // Kill focused window
        if (LEFT_META && LEFT_SHIFT && event.keyCode() == KEY_W) {
            if (LSurface* focusedSurface = focus()) {
                if (focusedSurface->toplevel()) {
                    focusedSurface->toplevel()->closeRequest();
                }
            }
        }

        if (LEFT_META && event.keyCode() == KEY_W) {
            if (LSurface* focusedSurface = focus()) {
                if (focusedSurface->toplevel()) {
                    focusedSurface->toplevel()->();
                }
            }
        }

        if (LEFT_META && event.keyCode() == KEY_UP) {
            if (LSurface* focusedSurface = focus()) {
                if (focusedSurface->maximized()) {
                    focusedSurface->unsetMaximizedRequest();
                } else {
                    focusedSurface->setMaximizedRequest();
                }
            }
        }

        if (event.keyCode() == KEY_ESC && LEFT_CTRL && LEFT_SHIFT)
        {
            compositor()->finish();
            return;
        }
        
    }


    G::scene().handleKeyboardKeyEvent(event, SETTINGS_SCENE_EVENT_OPTIONS);

}
