const root = Blink.createView("View");
const text = Blink.createView("Text");
const btn  = Blink.createView("Button");

Blink.appendChild(root, text);
Blink.appendChild(root, btn);

Blink.render();
