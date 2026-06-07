const fs = require('fs');
const path = require('path');

const SOURCE = {
  id: 'grile_curs_sdd',
  file: 'C:\\Users\\fgghk\\Downloads\\SDD_curs_mcqs_MANUAL_CROSSCHECKED\\SDD_curs_mcqs_MANUAL_CROSSCHECKED\\outputs_txt_docx\\grile_curs_sdd.txt',
};

const siteRoot = path.resolve(__dirname, '..');

function normalizeText(text) {
  return String(text || '')
    .replace(/\r\n/g, '\n')
    .replace(/\r/g, '\n')
    .replace(/[ \t]+$/gm, '')
    .trim();
}

function splitQuestionBlocks(text) {
  const normalized = normalizeText(text);
  const matches = [...normalized.matchAll(/^Question\s+(\d+)\b.*$/gmi)];
  if (!matches.length) {
    throw new Error('No "Question N" blocks found.');
  }

  return matches.map((match, index) => {
    const start = match.index + match[0].length;
    const end = index + 1 < matches.length ? matches[index + 1].index : normalized.length;
    return {
      sourceQuestionNumber: Number(match[1]),
      body: normalized.slice(start, end).trim(),
    };
  });
}

function parseCorrectLine(lines, sourceQuestionNumber) {
  const correctIndex = lines.findIndex(line => /^Correct\s*:/i.test(line.trim()));
  if (correctIndex === -1) {
    throw new Error(`Question ${sourceQuestionNumber}: missing Correct line.`);
  }

  const correctLine = lines[correctIndex].trim();
  const correctTokens = [...correctLine.matchAll(/\b([a-zA-Z]|\d+)\b/g)].map(match => match[1].toLowerCase());
  if (!correctTokens.length) {
    throw new Error(`Question ${sourceQuestionNumber}: Correct line has no option references.`);
  }

  return {
    correctIndex,
    correctTokens: new Set(correctTokens),
  };
}

function parseOptions(lines, sourceQuestionNumber) {
  const firstOptionIndex = lines.findIndex(line => /^\s*\d+\.\s+/.test(line));
  if (firstOptionIndex === -1) {
    throw new Error(`Question ${sourceQuestionNumber}: no numbered options found.`);
  }

  const questionLines = lines.slice(0, firstOptionIndex);
  const optionLines = lines.slice(firstOptionIndex);
  const options = [];

  for (const line of optionLines) {
    const optionMatch = line.match(/^\s*(\d+)\.\s*(.*)$/);
    if (optionMatch) {
      const labelMatch = optionMatch[2].match(/^\(([a-zA-Z])\)\s*(.*)$/);
      options.push({
        number: Number(optionMatch[1]),
        label: labelMatch ? labelMatch[1].toLowerCase() : null,
        textLines: [labelMatch ? labelMatch[2].trim() : optionMatch[2].trim()],
      });
      continue;
    }

    if (!options.length) {
      throw new Error(`Question ${sourceQuestionNumber}: option continuation before first option.`);
    }

    options[options.length - 1].textLines.push(line.trim());
  }

  const answers = options.map(option => ({
    number: option.number,
    label: option.label,
    text: option.textLines.filter(Boolean).join('\n').trim(),
  }));

  if (answers.some(answer => !answer.text)) {
    throw new Error(`Question ${sourceQuestionNumber}: blank option text.`);
  }

  return {
    questionText: questionLines.join('\n').trim(),
    answers,
  };
}

function parseQuestionBlock(block, index) {
  const lines = block.body.split('\n').map(line => line.trimEnd());
  const { correctIndex, correctTokens } = parseCorrectLine(lines, block.sourceQuestionNumber);
  const usefulLines = lines.slice(0, correctIndex).filter((line, lineIndex, allLines) => {
    if (line.trim()) return true;
    const prev = allLines[lineIndex - 1] || '';
    const next = allLines[lineIndex + 1] || '';
    return prev.trim() && next.trim();
  });

  const { questionText, answers } = parseOptions(usefulLines, block.sourceQuestionNumber);
  const optionNumbers = new Set(answers.map(answer => String(answer.number)));
  const optionLabels = new Set(answers.filter(answer => answer.label).map(answer => answer.label));
  const missingCorrects = [...correctTokens].filter(token => !optionNumbers.has(token) && !optionLabels.has(token));
  if (missingCorrects.length) {
    throw new Error(`Question ${block.sourceQuestionNumber}: Correct option(s) not present: ${missingCorrects.join(', ')}`);
  }

  return {
    id: index + 1,
    question: questionText,
    source: `${SOURCE.id}.txt`,
    sourceQuestionNumber: block.sourceQuestionNumber,
    answers: answers.map(answer => ({
      text: answer.text,
      isCorrect: correctTokens.has(String(answer.number)) || (answer.label ? correctTokens.has(answer.label) : false),
    })),
  };
}

function validate(questions) {
  if (!questions.length) {
    throw new Error(`${SOURCE.id}: extractor produced no questions.`);
  }

  const errors = [];
  questions.forEach((question, index) => {
    if (!question.question) errors.push(`${SOURCE.id} #${index + 1}: missing question text`);
    if (!Array.isArray(question.answers) || !question.answers.length) errors.push(`${SOURCE.id} #${index + 1}: missing answers`);
    if (question.answers.some(answer => !answer.text)) errors.push(`${SOURCE.id} #${index + 1}: blank answer text`);
    if (!question.answers.some(answer => answer.isCorrect)) errors.push(`${SOURCE.id} #${index + 1}: no correct answer`);
  });

  if (errors.length) {
    throw new Error(errors.join('\n'));
  }
}

function writeSet() {
  const rawText = fs.readFileSync(SOURCE.file, 'utf8');
  const blocks = splitQuestionBlocks(rawText);
  const questions = blocks.map((block, index) => parseQuestionBlock(block, index));
  validate(questions);

  const json = `${JSON.stringify(questions, null, 2)}\n`;
  const roPath = path.join(siteRoot, 'sets', `${SOURCE.id}.json`);
  const enPath = path.join(siteRoot, 'sets_en', `${SOURCE.id}.json`);
  fs.writeFileSync(roPath, json, 'utf8');
  fs.writeFileSync(enPath, json, 'utf8');

  const multiCorrect = questions.filter(question => question.answers.filter(answer => answer.isCorrect).length > 1).length;
  return {
    id: SOURCE.id,
    count: questions.length,
    singleCorrect: questions.length - multiCorrect,
    multiCorrect,
    roPath,
    enPath,
  };
}

const result = writeSet();
console.log(`${result.id}: ${result.count} questions (${result.singleCorrect} single-correct, ${result.multiCorrect} multi-correct)`);
console.log(`  wrote ${result.roPath}`);
console.log(`  wrote ${result.enPath}`);
