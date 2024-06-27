#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/widget.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>

// Define the App struct and function prototypes
typedef enum {
    FlipLinksMainMenuScene,
    FlipLinksPopupScene,
    FlipLinksInputScene,
    FlipLinksSceneCount,
} BasicScenesScene;

typedef enum {
    FlipLinksSubmenuView,
    FlipLinksWidgetView,
    FlipLinksTextInputView,
} BasicScenesView;

typedef struct App {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    Widget* widget;
    TextInput* text_input;
} App;

static App* app_alloc();
static void app_free(App* app);

static void fliplinks_menu_callback(void* context, uint32_t index) {
    UNUSED(context);
    UNUSED(index);
}

// Define scene handlers
void fliplinks_main_menu_scene_on_enter(void* context) {
    App* app = context;
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "FlipLinks");
    submenu_add_item(app->submenu, "Instagram", 0, fliplinks_menu_callback, app);
    submenu_add_item(app->submenu, "GitHub", 1, fliplinks_menu_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, FlipLinksSubmenuView);
}

bool fliplinks_main_menu_scene_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false; // event not handled
}

void fliplinks_main_menu_scene_on_exit(void* context) {
    UNUSED(context);
}

void fliplinks_popup_scene_on_enter(void* context) {
    UNUSED(context);
}

bool fliplinks_popup_scene_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false; // event not handled
}

void fliplinks_popup_scene_on_exit(void* context) {
    UNUSED(context);
}

void fliplinks_input_scene_on_enter(void* context) {
    UNUSED(context);
}

bool fliplinks_input_scene_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false; // event not handled
}

void fliplinks_input_scene_on_exit(void* context) {
    UNUSED(context);
}

void (*const fliplinks_scene_on_enter_handlers[])(void*) = {
    fliplinks_main_menu_scene_on_enter,
    fliplinks_popup_scene_on_enter,
    fliplinks_input_scene_on_enter,
};

bool (*const fliplinks_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    fliplinks_main_menu_scene_on_event,
    fliplinks_popup_scene_on_event,
    fliplinks_input_scene_on_event,
};

void (*const fliplinks_scene_on_exit_handlers[])(void*) = {
    fliplinks_main_menu_scene_on_exit,
    fliplinks_popup_scene_on_exit,
    fliplinks_input_scene_on_exit,
};

static const SceneManagerHandlers fliplinks_scene_manager_handlers = {
    .on_enter_handlers = fliplinks_scene_on_enter_handlers,
    .on_event_handlers = fliplinks_scene_on_event_handlers,
    .on_exit_handlers = fliplinks_scene_on_exit_handlers,
    .scene_num = FlipLinksSceneCount,
};

static bool fliplinks_custom_callback(void* context, uint32_t custom_event) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

bool fliplinks_back_event_callback(void* context) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static App* app_alloc() {
    App* app = malloc(sizeof(App));
    app->scene_manager = scene_manager_alloc(
        &fliplinks_scene_manager_handlers,
        app);
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, fliplinks_custom_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, fliplinks_back_event_callback);
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        FlipLinksSubmenuView,
        submenu_get_view(app->submenu));
    app->widget = widget_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        FlipLinksWidgetView,
        widget_get_view(app->widget));
    app->text_input = text_input_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        FlipLinksTextInputView,
        text_input_get_view(app->text_input));
    return app;
}

static void app_free(App* app) {
    furi_assert(app);
    view_dispatcher_remove_view(app->view_dispatcher, FlipLinksSubmenuView);
    view_dispatcher_remove_view(app->view_dispatcher, FlipLinksWidgetView);
    view_dispatcher_remove_view(app->view_dispatcher, FlipLinksTextInputView);
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);
    submenu_free(app->submenu);
    widget_free(app->widget);
    text_input_free(app->text_input);
    free(app);
}

int32_t fliplinks_app(void* p) {
    UNUSED(p);
    App* app = app_alloc();

    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(
        app->view_dispatcher,
        gui,
        ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(
        app->scene_manager,
        FlipLinksMainMenuScene);
    view_dispatcher_run(app->view_dispatcher);

    app_free(app);
    return 0;
}
