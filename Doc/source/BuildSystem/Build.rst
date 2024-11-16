Build
================

Problem
-------

When set the tracer data under cursor to execute from a timer, the the client cannot shoot properly.

Interface
---------

TODO: GetBuildable requirements

TODO
----

* Now the build system is based on task ``TargetDataUnderMouse``. Make it use ``WaitForInteractableTarget``.

* Remove shadow when build

* Set building HUD

* Set correct channel when finish buildable spawning ``EBuildableState::Placed``

* The buildable could be formed by some ``MeshComponent``

* When the placement is invalid, return the actor to original position if there is any. And check for validity of the original position.

* The buildable should implement Interactable interface

* Moving and rotating with an interpolation