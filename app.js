"use strict";

class No {
  constructor(codigo, nome) {
    this.codigo = codigo;
    this.nome = nome;
    this.prox = null;
  }
}

class Lista {
  constructor() {
    this.inicio = null;
    this.tamanho = 0;
  }

  buscar(codigo) {
    for (let atual = this.inicio; atual !== null; atual = atual.prox) {
      if (atual.codigo === codigo) return atual;
    }
    return null;
  }

  inserir(posicao, codigo, nome) {
    if (!Number.isInteger(posicao) || posicao < 0 || posicao > this.tamanho) return -1;
    if (this.buscar(codigo) !== null) return 0;

    const novo = new No(codigo, nome);
    if (posicao === 0) {
      novo.prox = this.inicio;
      this.inicio = novo;
    } else {
      let anterior = this.inicio;
      for (let i = 0; i < posicao - 1; i++) anterior = anterior.prox;
      novo.prox = anterior.prox;
      anterior.prox = novo;
    }
    this.tamanho++;
    return 1;
  }

  alterar(codigo, nome) {
    const no = this.buscar(codigo);
    if (no === null) return false;
    no.nome = nome;
    return true;
  }

  remover(codigo) {
    let atual = this.inicio;
    let anterior = null;

    while (atual !== null && atual.codigo !== codigo) {
      anterior = atual;
      atual = atual.prox;
    }
    if (atual === null) return false;

    if (anterior === null) this.inicio = atual.prox;
    else anterior.prox = atual.prox;
    this.tamanho--;
    return true;
  }
}

const lista = new Lista();
const status = document.querySelector("#status");
const records = document.querySelector("#records");
const emptyMessage = document.querySelector("#empty-message");
const count = document.querySelector("#list-count");
const positionInput = document.querySelector("#new-position");

function mostrarStatus(mensagem, erro = false) {
  status.textContent = mensagem;
  status.classList.toggle("error", erro);
}

function lerCodigo(input) {
  const codigo = Number(input.value);
  if (
    input.value.trim() === "" ||
    !Number.isInteger(codigo) ||
    codigo < -2147483648 ||
    codigo > 2147483647
  ) {
    return null;
  }
  return codigo;
}

function atualizarLista() {
  records.replaceChildren();
  let posicao = 0;

  for (let atual = lista.inicio; atual !== null; atual = atual.prox) {
    const linha = document.createElement("tr");
    for (const valor of [posicao++, atual.codigo, atual.nome]) {
      const celula = document.createElement("td");
      celula.textContent = String(valor);
      linha.append(celula);
    }
    records.append(linha);
  }

  count.textContent = `${lista.tamanho} ${lista.tamanho === 1 ? "elemento" : "elementos"}`;
  emptyMessage.hidden = lista.tamanho !== 0;
  positionInput.max = String(lista.tamanho);
}

function incluir(modo) {
  const codigo = lerCodigo(document.querySelector("#new-code"));
  const nomeInput = document.querySelector("#new-name");
  const nome = nomeInput.value.trim();
  const posicaoInformada = Number(positionInput.value);

  if (codigo === null) {
    mostrarStatus("Informe um código inteiro válido.", true);
    return;
  }
  if (nome === "") {
    mostrarStatus("O nome não pode ser vazio.", true);
    return;
  }

  let posicao;
  if (modo === "start") posicao = 0;
  else if (modo === "end") posicao = lista.tamanho;
  else if (!Number.isInteger(posicaoInformada) || posicaoInformada < 0 || posicaoInformada > lista.tamanho) {
    mostrarStatus(`A posição deve estar entre 0 e ${lista.tamanho}.`, true);
    return;
  } else posicao = posicaoInformada;

  const resultado = lista.inserir(posicao, codigo, nome);
  if (resultado === 0) {
    mostrarStatus(`O código ${codigo} já existe.`, true);
    return;
  }

  atualizarLista();
  mostrarStatus(`Registro incluído na posição ${posicao}.`);
  document.querySelector("#new-code").value = "";
  nomeInput.value = "";
  document.querySelector("#new-code").focus();
}

document.querySelectorAll("[data-insert]").forEach((botao) => {
  botao.addEventListener("click", () => incluir(botao.dataset.insert));
});

document.querySelector("#search-form").addEventListener("submit", (event) => {
  event.preventDefault();
  const codigo = lerCodigo(document.querySelector("#search-code"));
  const resultado = document.querySelector("#search-result");

  if (codigo === null) {
    resultado.textContent = "Informe um código inteiro válido.";
    return;
  }
  const no = lista.buscar(codigo);
  resultado.textContent = no === null
    ? `Código ${codigo} não encontrado.`
    : `Encontrado — Código: ${no.codigo} | Nome: ${no.nome}`;
});

document.querySelector("#edit-form").addEventListener("submit", (event) => {
  event.preventDefault();
  const codigo = lerCodigo(document.querySelector("#edit-code"));
  const nome = document.querySelector("#edit-name").value.trim();

  if (codigo === null) {
    mostrarStatus("Informe um código inteiro válido.", true);
    return;
  }
  if (nome === "") {
    mostrarStatus("O nome não pode ser vazio.", true);
    return;
  }
  if (!lista.alterar(codigo, nome)) {
    mostrarStatus(`Código ${codigo} não encontrado.`, true);
    return;
  }
  atualizarLista();
  mostrarStatus(`Registro ${codigo} alterado com sucesso.`);
  document.querySelector("#edit-name").value = "";
});

document.querySelector("#remove-form").addEventListener("submit", (event) => {
  event.preventDefault();
  const codigo = lerCodigo(document.querySelector("#remove-code"));

  if (codigo === null) {
    mostrarStatus("Informe um código inteiro válido.", true);
    return;
  }
  if (!lista.remover(codigo)) {
    mostrarStatus(`Código ${codigo} não encontrado.`, true);
    return;
  }
  atualizarLista();
  mostrarStatus(`Registro ${codigo} removido.`);
});

atualizarLista();
