print "----------------------- Helloworld -----------------------";

-- Globals variants
-- number nl_Delta: Is current update time interval with ms.
print(nl_Delta);
-- userdata<NLActor> nl_Actor: Is the script's actor object.
print(nl_Actor);
-- userdata<NLScene> nl_Scene: Is the script's actor's scene object.
print(nl_Scene);
-- userdata<NLScript> nl_Script: Is this script object.
print(nl_Script);

--[[
userdata<NLActor> new_NLActor(table properties);
void delete_NLActor(userdata<NLActor>);

userdata<NLRigidbody> new_NLRigidbody(table properties);
void delete_NLRigidbody(userdata<NLRigidbody>);

userdata<NLComponent> new_NLComponent(table properties);
void delete_NLComponent(userdata<NLComponent>);

userdata<NLScript> new_NLScript(table properties);
void delete_NLScript(userdata<NLScript>);

userdata<NLSceneCamera> new_NLSceneCamera(userdata<NLScene> = nil, integer type = 1);
void delete_NLSceneCamera(userdata<NLSceneCamera>);

userdata<NLVector3> new_NLVector3(number x = (), number y = (), number z = ());

virtual metatable NLObject { -- NLObject is not a metatable
    string Name();
    string ClassName();
    boolean SetEnabled(boolean);
    boolean IsEnabled();
    string|integer|number|boolean|userdata<T>|nil GetProperty(string, string typeName = "");
    boolean RemoveProperty(string);
    boolean SetProperty(string, string|integer|number|boolean|userdata<T>, string typeName = "");
    boolean is_success[, string|integer|number|boolean|userdata|nil return_value] Invoke(string method_sign, string return_type, ...arguments);
};

metatable NLActor : NLObject{
    number x, number y, number z Position();
    number x, number y, number z Rotation();
    number x, number y, number z Scale();
    boolean SetPosition(number x, number y, number z);
    boolean SetRotation(number x, number y, number z);
    boolean SetScale(number x, number y, number z);
    boolean Move(number x, number y, number z);
    boolean Turn(number x, number y, number z);
    boolean Zoom(number x, number y, number z);
    number x, number y, number z Direction();
    number x, number y, number z Up();
    number x, number y, number z Right();

    userdata<NLScene> Scene();
    boolean CanRender();

    integer ChildrenCount();
    userdata<NLActor> GetChild(integer | string);
    userdata<NLActor> ParentActor();
    boolean AddChild(userdata<NLActor>);
    boolean RemoveChild(integer | string | userdata<NLActor>);
    userdata<NLActor> CreateChild(table properties);

    integer ComponentCount();
    userdata<NLComponent> GetComponent(integer | string);
    boolean AddComponent(userdata<NLComponent>);
    boolean RemoveComponent(integer | string | userdata<NLComponent>);
    userdata<NLComponent> CreateComponent(table properties);

    boolean AddScript(userdata<NLScript>);
    boolean RemoveScript(integer | string | userdata<NLScript>);
    integer ScriptCount();
    userdata<NLScript> GetScript(integer | string);
    userdata<NLScript> CreateScript(table properties);

    boolean IsRigidbody();
    userdata<NLRigidbody> ToRigidbody();
};

metatable NLRigidbody : NLActor {
    number x, number y, number z MoveDirection();
    boolean IsEnabled();
};

metatable NLScene {
    boolean SetClearColor(number r, number g, number b, number a);
    userdata<NLActor> GetActor(integer | string);
    integer ActorCount();
    number CurrentFPS();
    boolean MoveCursorToCenter();
    boolean SetCursorVisible(boolean);
    boolean GrabMouseCursor(boolean);
    boolean IsCursorVisible();
    boolean IsGrabMouseCursor();
    number CurrendDelta();
    boolean KeyState(integer | string);
    boolean MouseState(integer | string);
    userdata<NLSceneCamera> CurrentCamera();
    integer x, integer y MousePointerPosition();
    boolean KeyPressed(integer | string);
    boolean MousePressed(integer | string);
    boolean SetCurrentCamera(userdata<NLSceneCamera>);
    boolean AddActor(userdata<NLActor>);
    boolean RemoveActor(integer | string | userdata<NLActor>);
    userdata<NLActor> CreateActor(table properties);
};

metatable NLComponent : NLObject {
    userdata<NLActor> Actor();
};

metatable NLScript : NLObject {
    userdata<NLActor> Actor();
    // boolean SetScriptFile(string);
    // boolean SetScriptSource(string);
};

metatable NLSceneCamera {
    integer Type();
    boolean IsEnabled();
    boolean SetType(integer);
    boolean SetEnabled(boolean);
    userdata<NLScene> Scene();
    number x, number y, number z Position();
    number x, number y, number z Rotation();
    number x, number y, number z Scale();
    boolean SetPosition(number x, number y, number z);
    boolean SetRotation(number x, number y, number z);
    boolean SetScale(number x, number y, number z);
    boolean Move(number x, number y, number z);
    boolean Turn(number x, number y, number z);
    boolean Zoom(number x, number y, number z);
    number x, number y, number z Direction();
    number x, number y, number z Up();
    number x, number y, number z Right_XPositive();
    boolean ZIsUp();
    boolean SetZIsUp(boolean);
    boolean UpdateCamera(userdata<NLActor>);
};

metatable NLVector3 {
    number X(number x = ()); // getter/setter
    number Y(number y = ()); // getter/setter
    number Z(number z = ()); // getter/setter
    number, number, number XYZ(number x = (), number y = (), number z = ()); // getter/setter
    __gc;
};

]]

-- Register global variant to NLScript parent object, it will show on editor.
global_var_i = 123;

-- If Init() function is declared, it will execute automatic when first running this script.
-- return boolean: true - Init success. false - Init fail, the script will not continue running.
function Init()
-- Register global variant to NLScript parent object in Init() function, it also will show on editor.
    global_var_s = "abc";
    print "Init()";
    return true;
end

-- If Update(number) function is declared, it will execute when NLScript updating. If not, NLScript will run this whole lua script.
-- delta: number - equals nl_Delta
-- noreturn.
function Update(delta)
    print("Update()", delta);
end

-- If Destroy() function is declared, it will execute automatic before lua_State destroying.
-- noreturn.
function Destroy()
    print "Destroy()";
end

-- If Reset() function is declared, it will automatic before NLScript reset.
-- noreturn.
function Reset()
    print "Reset()";
end

print "----------------------- END -----------------------";

collectgarbage("collect");
