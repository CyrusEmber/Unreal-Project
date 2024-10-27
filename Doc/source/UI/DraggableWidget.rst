Draggable Widet
================

Base
-----------

The ""DraggableBase"" is the base for all draggable widget.

Popup
-----
``WBP_InventoryPopup`` is the popup for inventory.
* Remove the widget when lost focus, need to set the widget is focusable

Problem
-----------

**Q: "OnDrop" override on the inherited classes has the parent "CanDropOnDrag" effect?**

   **A:** Yes

** When I create ``WBP inventory Popup widget`` in ``InventorySlot``, get screen position from mouse event does not generating the correct location even after I removed DPI scale.

TODO
-----------

Make a subwidget of draggable widget that is right mouse clickable, with a drop down menu.