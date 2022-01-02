print "-----------------------Helloworld--------------------";

-- Globals variants
-- number nl_Delta: Is update time interval with ms.
print(nl_Delta);
-- userdata<NLActor> nl_Actor: Is the script's actor object.
print(nl_Actor);
-- userdata<NLScene> nl_Scene: Is the script's actor's scene object.
print(nl_Scene);

--[[
metatable NLActor {
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

    string Name();
    string ClassName();
    boolean SetEnabled(boolean);
    userdata<NLScene> Scene();
    boolean IsEnabled();
    boolean CanRender();

    integer ChildrenCount();
    userdata<NLActor> GetChild(integer | string);
    userdata<NLActor> ParentActor();
    integer ComponentCount();
    userdata<NLComponent> GetComponent(integer | string);
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
    boolean KeyState(integer);
    boolean MouseState(integer);
    userdata<NLSceneCamera> CurrentCamera();
    integer x, integer y MousePointerPosition();
};

metatable NLComponent {
    string Name();
    string ClassName();
    boolean SetEnabled(boolean);
    userdata<NLActor> Actor();
    boolean IsEnabled();
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
};

]]

-- If Init() function is declared, it will execute automatic when first running this script.
-- return boolean: true - Init success. false - Init fail, the script will not continue running.
function Init()
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

print "-------------------- END --------------------";

collectgarbage("collect");
