document.addEventListener("DOMContentLoaded", function() {
    function fetchLogs() {
        fetch('/logs')
            .then(response => response.json())
            .then(data => {
                const logContainer = document.getElementById('logs');
                logContainer.innerHTML = data.join('<br>');
                logContainer.scrollTop = logContainer.scrollHeight;
            });
        setTimeout(fetchLogs, 1000);
    }

    function getAdviceFromAI() {
        const adviceElement = document.getElementById('advice');
        adviceElement.innerHTML = "<p>Loading...</p>"; // Show loading text

        fetch('/send_to_llm', { method: 'POST' })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    adviceElement.innerText = data.response;
                } else {
                    adviceElement.innerText = 'Error: ' + data.error;
                }
            })
            .catch(error => {
                adviceElement.innerText = 'Error fetching advice.';
                console.error('Error:', error);
            });
    }

    document.getElementById('getAdviceBtn').addEventListener('click', getAdviceFromAI);
    fetchLogs();
});
