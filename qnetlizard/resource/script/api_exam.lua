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


--print(nl_Scene:ActorCount(), nl_Scene:IsCursorVisible(), nl_Scene:IsGrabMouseCursor());
--print(nl_Scene:CurrentFPS(), nl_Scene:CurrendDelta());
--print(nl_Scene:KeyState(65), nl_Scene:MouseState(1));

print(nl_Actor:ParentActor());
print(nl_Actor:GetChild(1):Name());
print(nl_Actor:Scene():CurrentFPS());
print(nl_Actor:GetComponent(0):Actor());
print(nl_Scene:ActorCount(), nl_Scene:GetActor(0):Name(), nl_Scene:CurrentCamera(), nl_Scene:GetActor(2):GetComponent(0):Name());

print "--------------------END-------------------";

collectgarbage("collect");
