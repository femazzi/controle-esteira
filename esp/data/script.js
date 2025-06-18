document.addEventListener('DOMContentLoaded', () => {
    const motorStatus = document.getElementById('motor-status');
    const motorText = document.getElementById('motor-text');
    const toggleButton = document.getElementById('toggle-motor');
    const pecasCount = document.getElementById('pecas-count');
    const resetButton = document.getElementById('reset-counter');

    // Função para atualizar o status
    async function atualizarStatus() {
        try {
            const response = await fetch('/status');
            const data = await response.json();
            
            // Atualiza o contador de peças
            pecasCount.textContent = data.pecas;
            
            // Atualiza o status do motor
            motorStatus.classList.toggle('active', data.motor === 'ligado');
            motorText.textContent = data.motor.charAt(0).toUpperCase() + data.motor.slice(1);
        } catch (error) {
            console.error('Erro ao atualizar status:', error);
        }
    }

    // Função para alternar o motor
    async function alternarMotor() {
        try {
            const response = await fetch('/toggle-motor', { method: 'POST' });
            const data = await response.json();
            
            // Atualiza a interface após alternar o motor
            atualizarStatus();
        } catch (error) {
            console.error('Erro ao alternar motor:', error);
        }
    }

    // Função para resetar o contador
    async function resetarContador() {
        try {
            const response = await fetch('/reset-counter', { method: 'POST' });
            const data = await response.json();
            
            // Atualiza a interface após resetar o contador
            atualizarStatus();
        } catch (error) {
            console.error('Erro ao resetar contador:', error);
        }
    }

    // Adiciona eventos de clique aos botões
    toggleButton.addEventListener('click', alternarMotor);
    resetButton.addEventListener('click', resetarContador);

    // Atualiza o status a cada segundo
    setInterval(atualizarStatus, 1000);

    // Atualiza o status inicial
    atualizarStatus();
}); 