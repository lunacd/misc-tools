FROM node:22 AS base

FROM base AS deps
WORKDIR /frontend
COPY ./frontend/package.json ./frontend/pnpm-lock.yaml ./
RUN corepack enable pnpm && pnpm i

FROM base AS builder
WORKDIR /frontend
COPY --from=deps /frontend/node_modules ./node_modules
COPY ./frontend ./
RUN npm run build

FROM base AS runner

RUN addgroup --gid 1001 --system nodejs
RUN adduser --system nextjs --uid 1001

WORKDIR /frontend

COPY --from=builder --chown=nextjs:nodejs /frontend/.next/standalone ./
COPY --from=builder --chown=nextjs:nodejs /frontend/.next/static ./.next/static

USER nextjs
EXPOSE 3000
ENV PORT=3000
ENV HOSTNAME="0.0.0.0"

CMD [ "node", "server.js" ]
