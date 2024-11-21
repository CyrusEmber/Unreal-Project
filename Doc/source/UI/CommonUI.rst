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

* Reactivate the menu widget create a new widget with tab. Fixed with ``SetInputMode``.

* Focus??

* Play with different focus settings

Workflow:
=========

``PrimariGameLayout``
---------------------
Stores the layout of different Stacks. And you can register layer with layer widget. Different stacks are distinguished by different Gameplay Tag starting with ``UI.Layer``.
Have to register layer before hand.
``BinggyUIPolicy``
------------------
Interact with ``LocalPlayer`` and ``UBinggyUIManagerSubsystem``, serve as a middle point.
Should bind ``NotifyPlayerAdded`` with the ``PlayerControllerSet``. 

``UIManageSubsystem``
---------------------
The ``UIManageSubsystem`` is used by ``BinggyGameInstance``, add UI to local player.

https://x157.github.io/UE5/LyraStarterGame/CommonUI/

.. _TabListView-label:

``TabListView``
---------------
``TabListView`` creates a tab list that is capable of switching the tab buttons based on different actions.
The tab button should implement interface ``IBinggyTabButtonInterface``,
which enabling setting button text based on the interface function.
Additionally ``FBinggyTabDescriptor`` holds the information for a tab button.

Functionality:
^^^^^^^^^^^^^^
:ref:`TabListView-label` can holds some pre-registered tabs, remember to set the button that implements ``IBinggyTabButtonInterface``.
When open the tab list, it remembers the last active tab by setting ``SetSelectionRequired`` to false.

``SetupTab`` initialize all the pre-registered tabs.

TODOs:
^^^^^^
* ``AddDynamicTab`` not working.

* Hidden tabs not working.

* Should have a data structure to hold all the tabs, and set the visibility based on the game.

Tips:
-----

Avoid using ``ActivatableWidget`` than ``CommonUserWidget``. And suspend input if possible.


