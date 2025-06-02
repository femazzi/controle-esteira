document.addEventListener('DOMContentLoaded', () => {
    const motorStatus = document.getElementById('motor-status');
    const motorText = document.getElementById('motor-text');
    const toggleButton = document.getElementById('toggle-motor');
    const pecasCount = document.getElementById('pecas-count');
    const resetButton = document.getElementById('reset-counter');
    const velocidadeSlider = document.getElementById('velocidade-slider');
    const velocidadeValor = document.getElementById('velocidade-valor');

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

            // Atualiza o slider se o valor for diferente
            if (parseInt(velocidadeSlider.value) !== data.velocidade) {
                velocidadeSlider.value = data.velocidade;
                velocidadeValor.textContent = `${data.velocidade}%`;
            }
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

    // Função para atualizar a velocidade do motor
    async function atualizarVelocidade(valor) {
        try {
            const response = await fetch('/velocidade', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `valor=${valor}`
            });
            const data = await response.json();
            velocidadeValor.textContent = `${valor}%`;
        } catch (error) {
            console.error('Erro ao atualizar velocidade:', error);
        }
    }

    // Adiciona eventos de clique aos botões
    toggleButton.addEventListener('click', alternarMotor);
    resetButton.addEventListener('click', resetarContador);

    // Adiciona evento para o slider
    let timeoutId = null;
    velocidadeSlider.addEventListener('input', (e) => {
        const valor = e.target.value;
        velocidadeValor.textContent = `${valor}%`;
        
        // Debounce para não sobrecarregar o servidor
        if (timeoutId) clearTimeout(timeoutId);
        timeoutId = setTimeout(() => {
            atualizarVelocidade(valor);
        }, 100);
    });

    // Atualiza o status a cada segundo
    setInterval(atualizarStatus, 1000);

    // Atualiza o status inicial
    atualizarStatus();
}); 