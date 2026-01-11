#include <iostream>
#include <cstring>
#include "ViewRegistry.h"
#include <fstream>
#include <sstream>

extern "C" {
#include "../js/quickjs/quickjs.h"
}

std::string loadFile(const std::string& path){
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/* =========================
   Utilidad: print(...)
   ========================= */
static JSValue js_print(JSContext *ctx,
                        JSValueConst this_val,
                        int argc,
                        JSValueConst *argv) {

    for (int i = 0; i < argc; i++) {
        const char *str = JS_ToCString(ctx, argv[i]);
        if (!str)
            return JS_EXCEPTION;

        std::cout << str;
        JS_FreeCString(ctx, str);

        if (i + 1 < argc)
            std::cout << " ";
    }

    std::cout << std::endl;
    return JS_UNDEFINED;
}

/* =========================
   Blink.createView(...)
   ========================= */
static int nextViewId = 1;

static JSValue js_createView(JSContext *ctx,
                             JSValueConst,
                             int argc,
                             JSValueConst *argv) {

    if (argc < 1)
        return JS_ThrowTypeError(ctx, "createView(type)");

    const char *type = JS_ToCString(ctx, argv[0]);
    if (!type)
        return JS_EXCEPTION;

    View* view = ViewRegistry::create(type);

    std::cout << "[Blink] createView "
              << type
              << " id=" << view->id << std::endl;

    JS_FreeCString(ctx, type);

    return JS_NewInt32(ctx, view->id);
}

/* ====================
    Append Child
   ====================*/
static JSValue js_appendChild(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv){
    if(argc < 2)
        return JS_ThrowTypeError(ctx, "appendChild(parentId, childId)");

    int parentId, childId;
    JS_ToInt32(ctx, &parentId, argv[0]);
    JS_ToInt32(ctx, &childId, argv[1]);

    View* parent = ViewRegistry::get(parentId);
    View* child = ViewRegistry::get(childId);

    if(!parent || !child)
        return JS_ThrowReferenceError(ctx, "Invalid view ID");
    
    parent->children.push_back(child);

    std::cout << "[Blink] appendChild parentId=" << parentId
              << " childId=" << childId << std::endl;

    return JS_UNDEFINED;
}

/* =========================
   Main runtime
   ========================= */
int main() {
    /* Runtime */
    JSRuntime *rt = JS_NewRuntime();
    JSContext *ctx = JS_NewContextRaw(rt);

    /* Intrinsics (contrato correcto) */
    JS_AddIntrinsicBaseObjects(ctx);
    JS_AddIntrinsicDate(ctx);
    JS_AddIntrinsicEval(ctx);
    JS_AddIntrinsicStringNormalize(ctx);
    JS_AddIntrinsicRegExp(ctx);
    JS_AddIntrinsicJSON(ctx);
    JS_AddIntrinsicProxy(ctx);
    JS_AddIntrinsicMapSet(ctx);
    JS_AddIntrinsicTypedArrays(ctx);
    JS_AddIntrinsicPromise(ctx);

    /* Global object */
    JSValue global = JS_GetGlobalObject(ctx);

    /* print(...) */
    JS_SetPropertyStr(
        ctx,
        global,
        "print",
        JS_NewCFunction(ctx, js_print, "print", 1)
    );
    

    /* Blink namespace */
    JSValue blink = JS_NewObject(ctx);

    JS_SetPropertyStr(
        ctx,
        blink,
        "createView",
        JS_NewCFunction(ctx, js_createView, "createView", 2)
    );

    JS_SetPropertyStr(
        ctx,
        blink,
        "appendChild",
        JS_NewCFunction(ctx, js_appendChild, "appendChild", 3)
    );

    JS_SetPropertyStr(
        ctx,
        global,
        "Blink",
        blink
    );

    JS_FreeValue(ctx, global);

    /* Test script */
    /*
    const char *script = R"(
        const id = Blink.createView("Text", { text: "Hola Blink" });
        print("View ID:", id);
    )";

    JS_Eval(ctx, script, std::strlen(script),
            "<blink>", JS_EVAL_TYPE_GLOBAL);
    */

    std::string code = loadFile("examples/app.js");

    JSValue result = JS_Eval(
        ctx,
        code.c_str(),
        code.size(),
        "app.js",
        JS_EVAL_TYPE_GLOBAL
    );

    if(JS_IsException(result)) {
        JSValue exception = JS_GetException(ctx);
        const char *errorMsg = JS_ToCString(ctx, exception);
        std::cerr << "JavaScript Exception: " << errorMsg << std::endl;
        JS_FreeCString(ctx, errorMsg);
        JS_FreeValue(ctx, exception);
    }

    /* Cleanup */
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    return 0;
}

