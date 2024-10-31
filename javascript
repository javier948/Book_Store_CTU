// Load inventory
async function loadInventory() {
    try {
        const response = await fetch('/api/inventory'); // Adjust API endpoint
        if (!response.ok) throw new Error('Failed to load inventory');
        
        const books = await response.json();
        const inventoryList = document.getElementById("inventoryList");
        inventoryList.innerHTML = books.map(book => `
            <div class="book-item">
                <p><strong>Title:</strong> ${book.title}</p>
                <p><strong>Author:</strong> ${book.author}</p>
                <p><strong>Edition:</strong> ${book.edition}</p>
                <p><strong>Genre:</strong> ${book.genre}</p>
                <p><strong>Quantity:</strong> ${book.quantity}</p>
            </div>
        `).join('');
    } catch (error) {
        console.error(error);
    }
}

// Place an order
async function placeOrder() {
    const title = document.getElementById("orderTitle").value;
    try {
        const response = await fetch('/api/place_order', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ title })
        });
        document.getElementById("orderStatus").innerText = await response.text();
    } catch (error) {
        console.error(error);
    }
}

// Add a new book
async function addBook() {
    const title = document.getElementById("bookTitle").value;
    const author = document.getElementById("bookAuthor").value;
    const edition = parseInt(document.getElementById("bookEdition").value);
    const genre = document.getElementById("bookGenre").value;
    const quantity = parseInt(document.getElementById("bookQuantity").value);

    try {
        const response = await fetch('/api/add_book', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ title, author, edition, genre, quantity })
        });
        document.getElementById("addBookStatus").innerText = await response.text();
    } catch (error) {
        console.error(error);
    }
}
