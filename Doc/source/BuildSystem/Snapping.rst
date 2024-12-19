Snapping:
=========

Bugs:
-----
* The preview mesh is missing even when Event ``LocalUpdatePreview`` is working properly. Problem: the ``WorldBuildable``
set its ``StaticMesh`` at ``BeginPlay``, which is later than the time the event is fired. As a result, the preview mesh
is set to be an empty mesh. Temporary fix: the ``WorldBuildable`` now have a default mesh, and the mesh initialization
at ``BeginPlay`` is removed (commented).

* Actor spawned in the client have collision. Problem: because collision channel ``OverlappingAll`` not overlapping
all custom channels.