CommonUI
========

https://dev.epicgames.com/documentation/en-us/unreal-engine/overview-of-advanced-multiplatform-user-interfaces-with-common-ui-for-unreal-engine

Tips
----
Widget stack or queue.

Selected: check box?

Get focus widget (override function), for gamepad. For example when the saved file is detected, continue button will show and the default focus is that.

Render focus rule, UI

Override ``Handle back action``

Bind gamepad icon in the widget

BUG:
----
For Lobby level, I changed level from none to text, and changed the level blueprint.

BUG1:
^^^^^

Set Brush for ``TabButton``.

Store tabs hidden somewhere.

Workflow:
=========

``PrimariGameLayout``
---------------------
Stores the layout of different Stacks. And you can register layer with layer widget. Different stacks are distinguished by different Gameplay Tag starting with ``UI.Layer``.

``BinggyUIPolicy``
------------------
Interact with ``LocalPlayer`` and ``UBinggyUIManagerSubsystem``, serve as a middle point

``UIManageSubsystem``
---------------------
The ``UIManageSubsystem`` is used by ``BinggyGameInstance``, add UI to local player.

https://x157.github.io/UE5/LyraStarterGame/CommonUI/


