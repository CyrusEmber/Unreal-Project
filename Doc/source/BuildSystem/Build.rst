Build
================

Problem
-------

* When set the tracer data under cursor to execute from a timer, the the client cannot shoot properly. Fixed.

* ``CalculateOffsetSpawnPoint`` Box Extent is wrong

* ``LineTrace`` trace response? Need overlapping result?



Interface
---------

TODO: GetBuildable requirements

Abilities:
----------
``Ability_Build``: it is a per-actor ability that replicates its buildable variable from client to the server.
TODO: Make buildable local spawn and then replicates to the server. Not directly modify the server data which cause
delay for local player.

TODO
----

* Now the build system is based on task ``TargetDataUnderMouse``. Make it use ``WaitForInteractableTarget``.

* Remove shadow when build *Done*

* Set building HUD

* Set correct channel when finish buildable spawning ``EBuildableState::Placed``

* The buildable could be formed by some ``MeshComponent``

* When the placement is invalid, return the actor to original position if there is any. And check for validity of the original position.

* The buildable should implement Interactable interface

* Moving and rotating with an interpolation

* The buildable spin in ``WorldBuildable`` now is using server RPC. Should local predict this.

* Create a manager class to handle build?

Bug:
----
* Only the server is capable of rotate buildable


